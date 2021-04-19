
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
                     DeviceCoordinates, PozyxSerial, get_first_pozyx_serial_port, SingleRegister)
from pythonosc.udp_client import SimpleUDPClient

from pypozyx.tools.version_check import perform_latest_version_check


#class MultitagPositioning(object):
#    """Continuously performs multitag positioning"""

def __init__(self, pozyx, osc_udp_client, tag_ids, anchors, algorithm=PozyxConstants.POSITIONING_ALGORITHM_UWB_ONLY,
                 dimension=PozyxConstants.DIMENSION_3D, height=1000):
        self.pozyx = pozyx
        self.osc_udp_client = osc_udp_client

        self.tag_ids = tag_ids
        self.anchors = anchors
        self.algorithm = algorithm
        self.dimension = dimension
        self.height = height
'''
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
'''
'''
def loop(self):
        """Performs positioning and prints the results."""
        for tag_id in self.tag_ids:
            position = Coordinates()
            status = self.pozyx.doPositioning(
                position, self.dimension, self.height, self.algorithm, remote_id=tag_id)
            if status == POZYX_SUCCESS:
                #print("HELLOOO222")
                self.printPublishPosition(position, tag_id)
            else:
                #print("HELLOOO")
                self.printPublishErrorCode("positioning", tag_id)
'''
def printPublishPosition(self, position, network_id):
        """Prints the Pozyx's position and possibly sends it as a OSC packet"""
        if network_id is None:
            network_id = 0
        s = "POS ID: {}, x(mm): {}, y(mm): {}, z(mm): {}".format("0x%0.4x" % network_id,
                                                                 position.x, position.y, position.z)
        print(s)
        if self.osc_udp_client is not None:
            self.osc_udp_client.send_message(
                "/position", [network_id, position.x, position.y, position.z])

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


#############################################################################################################################
# Configuration function
def configuration(self):
    print("#### Configuration started")

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
    #tag_ids = [None, None]
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

    osc_udp_client = None
    if use_processing:
        osc_udp_client = SimpleUDPClient(ip, network_port)

    pozyx = PozyxSerial(serial_port)
    
    print("#### Configuration finished ")
    print("#### Setup started ")
        #"""Sets up the Pozyx for positioning by calibrating its anchor list."""
    print("------------POZYX MULTITAG POSITIONING V{} -------------".format(version))
    print("")
    print(" - System will manually calibrate the tags")
    print("")
    print(" - System will then auto start positioning")
    print("")
    if None in tag_ids:
        for device_id in tag_ids:
            pozyx.printDeviceInfo(device_id)
    else:
        for device_id in [None] + tag_ids:
            pozyx.printDeviceInfo(device_id)
    print("")
    print("------------POZYX MULTITAG POSITIONING V{} -------------".format(version))
    print("")
    #self.setAnchorsManual(save_to_flash=False)

    for tag_id in tag_ids:
        status = pozyx.clearDevices(tag_id)
        for anchor in anchors:
            status &= pozyx.addDevice(anchor, tag_id)
        if len(anchors) > 4:
            status &= pozyx.setSelectionOfAnchors(PozyxConstants.ANCHOR_SELECT_AUTO, len(anchors), remote_id=tag_id)
    # enable these if you want to save the configuration to the devices.
    '''
    if save_to_flash:
        self.pozyx.saveAnchorIds(tag_id)
        self.pozyx.saveRegisters([PozyxRegisters.POSITIONING_NUMBER_OF_ANCHORS], tag_id)

        self.printPublishConfigurationResult(status, tag_id)
    '''

    for anchor in anchors:
        print("ANCHOR,0x%0.4x,%s" % (anchor.network_id, str(anchor.pos)))
        if osc_udp_client is not None:
            osc_udp_client.send_message(
                "/anchor", [anchor.network_id, anchor.pos.x, anchor.pos.y, anchor.pos.z])
            sleep(0.025)

    print("#### Setup done")

    # Set network ID here
    network_id = None

    return pozyx, osc_udp_client, tag_ids, anchors, algorithm, dimension, height, network_id

#############################################################################################################################
# Loop function

def loopToGetPosition(self, pozyx, osc_udp_client, tag_ids, anchors, algorithm, dimension, height, network_id):
#def loopToGetPosition(self):
    print("Loop started") 
        #"""Performs positioning and prints the results."""
    for tag_id in tag_ids:
        position = Coordinates()
        status = pozyx.doPositioning(position, dimension, height, algorithm, remote_id=tag_id)
        if status == POZYX_SUCCESS:
            print("HELLOOO222")
            #printPublishPosition(position, tag_id)
            '''
            if network_id is None:
                network_id = 0
                s = "POS ID: {}, x(mm): {}, y(mm): {}, z(mm): {}".format("0x%0.4x" % network_id, position.x, position.y, position.z)
                print(s)
            if self.osc_udp_client is not None:
                self.osc_udp_client.send_message(
                "/position", [network_id, position.x, position.y, position.z])
            '''
        else:
            print("Error status")
            #printPublishPosition(position, tag_id)
            #printPublishErrorCode("positioning", tag_id)
            if network_id is None:
                network_id = 0
                s = "POS ID: {}, x(mm): {}, y(mm): {}, z(mm): {}".format("0x%0.4x" % network_id, position.x, position.y, position.z)
                print(s)
            if osc_udp_client is not None:
                osc_udp_client.send_message(
                "/position", [network_id, position.x, position.y, position.z])

#############################################################################################################################
if __name__ == "__main__":

    #configuration(self=1)
       
    pozyx, osc_udp_client, tag_ids, anchors, algorithm, dimension, height, network_id = configuration(self=1)
    print("Variables returned ")
    print(tag_ids)

    #loopToGetPosition(pozyx, osc_udp_client, tag_ids, anchors, algorithm, dimension, height)
    loopToGetPosition(self = 1, pozyx = pozyx, osc_udp_client = osc_udp_client, tag_ids = tag_ids, anchors = anchors, algorithm = algorithm, dimension = dimension, height = height, network_id = network_id)
    











    '''
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

    osc_udp_client = None
    if use_processing:
        osc_udp_client = SimpleUDPClient(ip, network_port)

    pozyx = PozyxSerial(serial_port)


    r = MultitagPositioning(pozyx, osc_udp_client, tag_ids, anchors, algorithm, dimension, height)
    r.setup()
    while True:
        r.loop()
    '''