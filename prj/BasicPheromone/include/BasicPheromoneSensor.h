/*
    Copyright 2013 <copyright holder> <email>

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/


#ifndef BASICPHEROMONESENSOR_H
#define BASICPHEROMONESENSOR_H

#include "WorldModels/RobotAgentWorldModel.h"


class BasicPheromoneSensor
{

  public:
    BasicPheromoneSensor(RobotAgentWorldModel *__wm);
    virtual ~BasicPheromoneSensor();
    
    void update();
    
    /*
     * 
     * First Dimension: 
     * Sensor#. 0 Front. #1: 45deg, #2: 90 deg, ... ,#7: 315 deg
     * Second Dimension: 
     * 0: Degrees from front. 
     * 1: Relative orientation
     * 2: x coordinate of light sensor
     * 3: y coordinate of light sensor
     * 
     * 4: Distance to nearest wall (Black pixel is gForegroundImage)
     * 5: Light Sensor value (Normalized alpha value of gZoneImage)
     */
    double _sensor[8][6];
    
        
    int foodFound();
    
    int getIndexOfMaxLeftLightSensor();
    int getIndexOfMaxRightLightSensor();
    
    double getRightLightMax();
    double getLeftLightMax();
    
    
    


  protected:
    
    const static double pi = 3.141592;

    double lengthLight;
    
    const static int x_offset = 0;
    const static int y_offset = 0;
    
    int x_center;
    int y_center;
    
    
    double sine(double theta);
    double cosine(double theta);
    
    double getLightSensorValue(int x, int y);
    
    
    RobotAgentWorldModel *_wm;
    
    



};

#endif // BASICPHEROMONESENSOR_H
