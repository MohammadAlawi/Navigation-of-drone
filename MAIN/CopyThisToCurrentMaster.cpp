    switch (inputChar) {

      case 'a':
        vehicle->control->armMotors(1);                                         // Call method from FlightCommander class that commands vehicle to force landing
        break;

      case 'l':
        flightcommander->ForceLanding(vehicle);                                 // Call method from FlightCommander class that commands vehicle to force landing
        break;

      case 't' :
        // Takeoff code here
        for(int i = 0; i < 4000; i++)
        {
          vehicle->control->attitudeAndVertPosCtrl(0,0,-63,2);
          usleep(1000);
        }
        for(int i = 0; i < 5000; i++)
        {
          vehicle->control->attitudeAndVertPosCtrl(0,-7,-63,0.5);                  // Y is inversed
          usleep(100);
        }

        flightcommander->ForceLanding(vehicle);                                 // Call method from FlightCommander class that commands vehicle to force landing
        break;

      case 'm' :
        // Move code here
        flighttelemetry->GetGlobalPositionData(vehicle, 1);
        for(int i = 0; i < 2000; i++)
        {
          vehicle->control->attitudeAndVertPosCtrl(0,-5,-63,0);                  // Y is inversed
          usleep(1000);
        }
                for(int i = 0; i < 2000; i++)

        flightcommander->ForceLanding(vehicle);                                 // Call method from FlightCommander class that commands vehicle to force landing
        break;

      case 'n' :
        // Move code here
        flighttelemetry->GetGlobalPositionData(vehicle, 1);
        for(int i = 0; i < 4000; i++)
        {
          //vehicle->control->attitudeAndVertPosCtrl(0,-3,-63,0);
          vehicle->control->positionAndYawCtrl(0,-20,0,-63);
          usleep(1000);
        }
        flightcommander->ForceLanding(vehicle);                                 // Call method from FlightCommander class that commands vehicle to force landing
      break;
      case 'b' :
        // Move code here
        flighttelemetry->GetGlobalPositionData(vehicle, 1);
        for(int i = 0; i < 2000; i++)
        {
          //vehicle->control->attitudeAndVertPosCtrl(-3,0,-63,0);
          vehicle->control->positionAndYawCtrl(-20,0,0,-63);
          usleep(1000);
        }
        flightcommander->ForceLanding(vehicle);                                 // Call method from FlightCommander class that commands vehicle to force landing
        break;

      case 'd' :
        // Data code here
        flighttelemetry->GetQuaternionData(vehicle);
        break;

    }
    if(inputChar == 'e')                                                        // End session here
    {
      break;
    }
  }
