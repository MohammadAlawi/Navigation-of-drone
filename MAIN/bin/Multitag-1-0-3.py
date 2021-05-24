
#!/usr/bin/env python
"""
The Pozyx ready to localize tutorial (c) Pozyx Labs
Please read the tutorial that accompanies this sketch:
https://www.pozyx.io/Documentation/Tutorials/ready_to_localize/Python

This tutorial requires at least the contents of the Pozyx Ready to Localize kit. It demonstrates the positioning capabilities
of the Pozyx device both locally and remotely. Follow the steps to correctly set up your environment in the link, change the
parameters and upload this sketch. Watch the coordinates change as you move your device around!

"""
from time import sleep

from pypozyx import (PozyxConstants, Coordinates, POZYX_SUCCESS, PozyxRegisters, version,
                     DeviceCoordinates, PozyxSerial, get_first_pozyx_serial_port, SingleRegister, SensorData)
from pythonosc.udp_client import SimpleUDPClient
from pypozyx.definitions.bitmasks import POZYX_INT_MASK_IMU

from pypozyx.tools.version_check import perform_latest_version_check

import os                                               # Include for Piping
from timeit import default_timer as timer               # Include for Measuring execution time

import numpy as np


class MultitagPositioning(object):
    """Continuously performs multitag positioning"""

    def __init__(self, pozyx, osc_udp_client, tag_ids, anchors, algorithm=PozyxConstants.POSITIONING_ALGORITHM_UWB_ONLY,
                 dimension=PozyxConstants.DIMENSION_3D, height=1000, initialization = False):                           # Added for initialization
        self.pozyx = pozyx
        self.osc_udp_client = osc_udp_client

        self.tag_ids = tag_ids
        self.anchors = anchors
        self.algorithm = algorithm
        self.dimension = dimension
        self.height = height
        self.initialization = initialization                                                                            # Added for initialization

    def setup(self):
        """Sets up the Pozyx for positioning by calibrating its anchor list."""
        print("------------POZYX MULTITAG POSITIONING V{} -------------".format(version))
        print("")
        print(" - System will manually calibrate the tags")
        print("")
        print(" - System will then auto start positioning")
        print("")
        if None in self.tag_ids:
            for device_id in self.tag_ids:
                self.pozyx.printDeviceInfo(device_id)
        else:
            for device_id in [None] + self.tag_ids:
                self.pozyx.printDeviceInfo(device_id)
        print("")
        print("------------POZYX MULTITAG POSITIONING V{} -------------".format(version))
        print("")

        self.setAnchorsManual(save_to_flash=False)

        self.printPublishAnchorConfiguration()

    def loop(self, initialization):
        """Performs positioning and prints the results."""
        for tag_id in self.tag_ids:
            position = Coordinates()
            status = self.pozyx.doPositioning(
                position, self.dimension, self.height, self.algorithm, remote_id=tag_id)
            if status == POZYX_SUCCESS:
                if initialization == False:
                    initialization = self.printPublishPosition(position, tag_id, initialization = False)    # Need to pass variable to function
                if initialization == True:
                    initialization = self.printPublishPosition(position, tag_id, initialization = True)    # Need to pass variable to function
            else:
                self.printPublishErrorCode("positioning", tag_id)

        print(initialization)

    def printPublishPosition(self, position, network_id, initialization):                       # Need to pass variable to function
        """Prints the Pozyx's position and possibly sends it as a OSC packet"""
        print("STEP 1")
        if initialization is False:                                                             # Start initialization if not done
            print("STEP 2")
            print("Piping Initialization Done")
            os.remove("Pipe.fifo")                                                              # Remove previous FIFO pipe
            path= "/home/uwb5/uwb/Onboard-SDK/build/binPipe.fifo"                               # Add path to pipe file
            os.mkfifo(path)                                                                     # Create FIFO pipe
            fifo=open(path,'w')                                                                 # Open FIFO pipe
            start = timer()                                                                     # Start high resolution timer (Optional)
            initialization = True                                                               # Set value such as initialization occours only once                                                 
                                                                         
        if network_id is None:
            network_id = 0
        #s = "POS ID {}, x(mm): {}, y(mm): {}, z(mm): {}".format("0x%0.4x" % network_id, position.x, position.y, position.z)
        s = "ID{} X{} Y{} Z{}".format("0x%0.4x" % network_id, position.x, position.y, position.z)
        print("STEP 2.5")
        fifo.write(s)                                                                       # Write to FIFO pipe
        #fifo.close()                                                                       # TODO: Test if fifo.close() affects on performance
        #print(s)                                                                           # Print results to terminal (Optional)
        end = timer()                                                                      # Take time using timer (Optional)
        #print(end - start) # Time in seconds, e.g. 5.38091952400282                        # Print execution time  (Optional)
        if self.osc_udp_client is not None:
            self.osc_udp_client.send_message("/position", [network_id, position.x, position.y, position.z])
        #print(initialization)
        print("STEP 3")

        return initialization

    def setAnchorsManual(self, save_to_flash=False):
        """Adds the manually measured anchors to the Pozyx's device list one for one."""
        for tag_id in self.tag_ids:
            status = self.pozyx.clearDevices(tag_id)
            for anchor in self.anchors:
                status &= self.pozyx.addDevice(anchor, tag_id)
            if len(anchors) > 4:
                status &= self.pozyx.setSelectionOfAnchors(PozyxConstants.ANCHOR_SELECT_AUTO, len(anchors),
                                                           remote_id=tag_id)
            # enable these if you want to save the configuration to the devices.
            if save_to_flash:
                self.pozyx.saveAnchorIds(tag_id)
                self.pozyx.saveRegisters([PozyxRegisters.POSITIONING_NUMBER_OF_ANCHORS], tag_id)

            self.printPublishConfigurationResult(status, tag_id)

    def printPublishConfigurationResult(self, status, tag_id):
        """Prints the configuration explicit result, prints and publishes error if one occurs"""
        if tag_id is None:
            tag_id = 0
        if status == POZYX_SUCCESS:
            print("Configuration of tag %s: success" % tag_id)
        else:
            self.printPublishErrorCode("configuration", tag_id)

    def printPublishErrorCode(self, operation, network_id):
        #Prints the Pozyx's error and possibly sends it as a OSC packet
        error_code = SingleRegister()
        status = self.pozyx.getErrorCode(error_code, network_id)
        if network_id is None:
            network_id = 0
        if status == POZYX_SUCCESS:
            #print("Error %s on ID %s, %s" %
                  #(operation, "0x%0.4x" % network_id, self.pozyx.getErrorMessage(error_code)))
            if self.osc_udp_client is not None:
                self.osc_udp_client.send_message(
                    "/error_%s" % operation, [network_id, error_code[0]])
        else:
            # should only happen when not being able to communicate with a remote Pozyx.
            self.pozyx.getErrorCode(error_code)
            #print("Error % s, local error code %s" % (operation, str(error_code)))
            if self.osc_udp_client is not None:
                self.osc_udp_client.send_message("/error_%s" % operation, [0, error_code[0]])


    def printPublishAnchorConfiguration(self):
        for anchor in self.anchors:
            print("ANCHOR,0x%0.4x,%s" % (anchor.network_id, str(anchor.pos)))
            if self.osc_udp_client is not None:
                self.osc_udp_client.send_message(
                    "/anchor", [anchor.network_id, anchor.pos.x, anchor.pos.y, anchor.pos.z])
                sleep(0.025)

    '''
    def MainFunction(self):
            # Check for the latest PyPozyx version. Skip if this takes too long or is not needed by setting to False.
        print("MainFunction called")
        check_pypozyx_version = False
        if check_pypozyx_version:
            perform_latest_version_check()

        # shortcut to not have to find out the port yourself.
        serial_port = get_first_pozyx_serial_port()
        if serial_port is None:
            print("No Pozyx connected. Check your USB cable or your driver!")
            quit()
        print("Step 1")
        # enable to send position data through OSC
        use_processing = True

        # configure if you want to route OSC to outside your localhost. Networking knowledge is required.
        ip = "127.0.0.1"
        network_port = 8888
        print("Step 2")

        # IDs of the tags to position, add None to position the local tag as well.
        tag_ids = [None, None]

        # necessary data for calibration
        anchors = [DeviceCoordinates(0x6776, 1, Coordinates(4250, 4500, 1760)),
                DeviceCoordinates(0x6a06, 1, Coordinates(60, 270, 80)),
                DeviceCoordinates(0x6a11, 1, Coordinates(0, 4440, 90)),
                DeviceCoordinates(0x6a2a, 1, Coordinates(0, 2180, 2050)),
                DeviceCoordinates(0x6a2d, 1, Coordinates(6840, 0, 1700)),
                DeviceCoordinates(0x6a46, 1, Coordinates(7000, 4330, 2530))]

        # positioning algorithm to use, other is PozyxConstants.POSITIONING_ALGORITHM_TRACKING
        algorithm = PozyxConstants.POSITIONING_ALGORITHM_UWB_ONLY
        # positioning dimension. Others are PozyxConstants.DIMENSION_2D, PozyxConstants.DIMENSION_2_5D
        dimension = PozyxConstants.DIMENSION_3D
        # height of device, required in 2.5D positioning
        height = 500
        print("Step 3")
        osc_udp_client = None
        if use_processing:
            osc_udp_client = SimpleUDPClient(ip, network_port)
        print("MainFunction finished")
        
        pozyx = PozyxSerial(serial_port)
        print(pozyx)
        print("MainFunction finished")

        r = MultitagPositioning(pozyx, osc_udp_client, tag_ids, anchors,
                            algorithm, dimension, height)
        r.setup()
        '''

def MyFunction(self):
    print("WORKS")


if __name__ == "__main__":

    #MyFunction(1)
    network_id = None
    
    # Check for the latest PyPozyx version. Skip if this takes too long or is not needed by setting to False.
    check_pypozyx_version = False
    if check_pypozyx_version:
        perform_latest_version_check()

    # shortcut to not have to find out the port yourself.
    serial_port = get_first_pozyx_serial_port()
    if serial_port is None:
        print("No Pozyx connected. Check your USB cable or your driver!")
        quit()

    # enable to send position data through OSC
    use_processing = True

    # configure if you want to route OSC to outside your localhost. Networking knowledge is required.
    ip = "127.0.0.1"
    network_port = 8888


    # IDs of the tags to position, add None to position the local tag as well.
    tag_ids = [None, 0x6e5a]

    # necessary data for calibration
    anchors = [DeviceCoordinates(0x6776, 1, Coordinates(4250, 4500, 1760)),
               DeviceCoordinates(0x6a06, 1, Coordinates(60, 270, 80)),
               DeviceCoordinates(0x6a11, 1, Coordinates(0, 4440, 90)),
               DeviceCoordinates(0x6a2a, 1, Coordinates(0, 2180, 2050)),
               DeviceCoordinates(0x6a2d, 1, Coordinates(6840, 0, 1700)),
               DeviceCoordinates(0x6a46, 1, Coordinates(7000, 4330, 2530))]

    # positioning algorithm to use, other is PozyxConstants.POSITIONING_ALGORITHM_TRACKING
    algorithm = PozyxConstants.POSITIONING_ALGORITHM_UWB_ONLY
    # positioning dimension. Others are PozyxConstants.DIMENSION_2D, PozyxConstants.DIMENSION_2_5D
    dimension = PozyxConstants.DIMENSION_3D
    # height of device, required in 2.5D positioning
    height = 500

    osc_udp_client = None
    if use_processing:
        osc_udp_client = SimpleUDPClient(ip, network_port)

    pozyx = PozyxSerial(serial_port)

    r = MultitagPositioning(pozyx, osc_udp_client, tag_ids, anchors,
                            algorithm, dimension, height)
    r.setup()

    initialization = False                                                                      # Create variable which indicates initialization status
    remote_id = None                                                                            # Add remote ID to be used for sensor data

    while True:
        #

        #
        #r.loop(initialization)
        start = timer()                                                                         # Start high resolution timer (Optional)
        # Sensor data START ******************************************************************************
        sensor_data = SensorData()
        calibration_status = SingleRegister()
        if remote_id is not None or pozyx.checkForFlag(POZYX_INT_MASK_IMU, 0.01) == POZYX_SUCCESS:
            status = pozyx.getAllSensorData(sensor_data, remote_id)
            status &= pozyx.getCalibrationStatus(calibration_status, remote_id)
            #print("Stage before POZYX_SUCCESS")
            #print(sensor_data.euler_angles.heading)
            '''
            if status == POZYX_SUCCESS:
                #publishSensorData(sensor_data, calibration_status)
                #print("Publish stage")
                #print(sensor_data)
                #print(sensor_data.quaternion.z)
                #print(sensor_data.euler_angles.heading)
                #print(sensor_data.acceleration.x)
                #print(sensor_data.linear_acceleration.x)
            '''
        # Sensor data END ******************************************************************************        
            
        #for tag_id in tag_ids:
            # Algorithm START ******************************************************************************
            #sleep(0.0000001)
            tag_1 = tag_ids[0]
            position1 = Coordinates()
            status1 = pozyx.doPositioning(
            position1, dimension, height, algorithm, remote_id=tag_1)

            tag_2 = tag_ids[1]
            position2 = Coordinates()
            status2 = pozyx.doPositioning(
            position2, dimension, height, algorithm, remote_id=tag_2)

        
            #differentiation of x, y, z between tag1 and tag2
            position_x_raw = (position1.x+position2.x)/2
            position_y_raw = (position1.y+position2.y)/2
            position_z = ((position1.z+15)+position2.z)/2

            initialize = False
            if initialize == False:
                A_k = np.array([[1.0, 0.0], [0.0, 1.0]])    #A matrix, express how the states x and y changes
                V_k = np.array([0.01, 0.01])                #noise applied to the forward kinematics
                Q_k = np.array([[1.0, 0], [0, 1.0]])         #state model noise covariance
                H_k = np.array([[1.0, 0], [0, 1.0]])        #convert the predicted sensor measurements at time k
                R_k = np.array([[1.0, 0], [0, 1.0]])        #sensor measurement noise covariance (if trust measurement it is close to zero)
                w_k = np.array([0.07, 0.07])                #sensor measurement (should be informed in the datasheet but pozyx has not noise data)
                u_k = np.array([4.5, 0])                    #control vector
                X_k = np.array([0, 0])                      #state estimate
                P_k = np.array([[0.01,0], [0, 0.01]])         
                #print("Initialization Done")
                initialize = True
             #state estimate
            X_k = A_k @ (X_k) + (u_k) + (V_k)

            #covariance of the state
            P_k = A_k @ P_k @ A_k + (Q_k)

            #######  measurement (upodate) ########
            # measurement value
            Z_k = np.array([position_x_raw, position_y_raw])

            #Y_k residual measurement
            Y_k =  Z_k - ((H_k @ X_k) + w_k)

            #residual covariance
            S_k = H_k @ P_k @ H_k + R_k

            #Kalman gain
            K_k = P_k @ H_k @ np.linalg.pinv(S_k)

            #updated state estimate
            X_k = X_k + (K_k @ Y_k)

            #updated covariance od the state
            P_X = P_k - (K_k @ H_k @ P_k)

            # position X, Y filtered
            position_x = X_k[0]*2
            position_y = X_k[1]*2
            #print("position offset X:" ,position_x, "position offset Y:" ,position_y)
            #just for plotting
            X = [1,2,3,4,5,6,7,8,9,10]
            Y1 =[5678.0,5797.5,5623.0,5578.0,5525.5,5615.0,5598.5,5564.5,5580.5,5700.5]
            Y2 =[5645.3,5721.4,5672.2,5625.1,5575.3,5595.1,5596.8,5580.6,5580.5,5640.]
            Y3 =[5665.0,5667.0,5678.5,5682.5,5601.5,5667.5,5768.0,5676.5,5673.5,5672.5]
            Y4 =[5702.8,5695.6,5692.2,5690.3,5672.5,5671.5,5690.8,5687.9,5685.0,5682.5]
            '''
            plt.plot(X, Y3, color="red")
            plt.plot(X, Y4, color="blue")
            plt.show()
            '''
            #print(s)
            # Algorithm END ******************************************************************************
                        
            #position = Coordinates()
            #status = pozyx.doPositioning(
            #    position, dimension, height, algorithm, remote_id=tag_id)
            
            # Piping START ******************************************************************************
            if status1 == POZYX_SUCCESS and status2 == POZYX_SUCCESS:
                s = "pX{}+pY{}+pZ{}+aX{}+aY{}+aZ{}+eH{}+".format("%0.1f" % position_x, position_y, position_z,
                sensor_data.linear_acceleration.x, sensor_data.linear_acceleration.y, sensor_data.linear_acceleration.z, sensor_data.euler_angles.heading)
                #print("X ",position_x," Y ",position_y," Z ", position_z)
                #print("STEP 1")
                if initialization is False:                                                             # Start initialization if not done
                    print("Piping Initialization Started")
                    os.remove("Pipe.fifo")                                                              # Remove previous FIFO pipe
                    path= "/home/uwb5/uwb/Onboard-SDK/build/bin/Pipe.fifo"                              # Add path to pipe file
                    os.mkfifo(path)                                                                     # Create FIFO pipe
                    print("Piping Initialization Finished")
                    print("Ready to Run") 
                    fifo=open(path,'w')                                                                 # Open FIFO pipe
                    initialization = True                                                               # Set value such as initialization occours only once                                                
                                                                                
                if network_id is None:
                    network_id = 0
                #s = "POS ID {}, x(mm): {}, y(mm): {}, z(mm): {}".format("0x%0.4x" % network_id, position.x, position.y, position.z)
                #s = "ID{} X{} Y{} Z{}".format("%0.f" % position_x, position_y, position_z)
                #print("STEP 2.5")
                fifo=open(path,'w')                                                                 # Open FIFO pipe
                fifo.write(s)                                                                       # Write to FIFO pipe
                fifo.close()                                                                        # Close FIFO pipe                       
                if osc_udp_client is not None:
                    osc_udp_client.send_message("/position", [network_id, position_x, position_y, position_z])
                #print(initialization)
                #print("STEP 3")

            else:
                #printPublishErrorCode("positioning", tag_id)
                s = "pX{}+pY{}+pZ{}+aX{}+aY{}+aZ{}+eH{}+".format("%0.1f" % position_x, position_y, position_z,
                sensor_data.linear_acceleration.x, sensor_data.linear_acceleration.y, sensor_data.linear_acceleration.x, sensor_data.euler_angles.heading)
                #s = "POS ID {}, x(mm): {}, y(mm): {}, z(mm): {}".format("0x%0.4x" % network_id, position.x, position.y, position.z)
                #s = "ID{} X{} Y{} Z{}".format("0x" % network_id, position.x, position.y, position.z)
                print("FAULT IN TAG SYSTEM !!!")
                if osc_udp_client is not None:
                    osc_udp_client.send_message("/position", [network_id, position_x, position_y, position_z])
            # Piping END ******************************************************************************
        
        end = timer()                                                                       # Take time using timer (Optional)
        print(1/(end - start))                                                                  # Print execution frequency  (Optional)
