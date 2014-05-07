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


#include "../include/BasicPheromoneSensor.h"
#include "World/World.h"


BasicPheromoneSensor::BasicPheromoneSensor(RobotAgentWorldModel *__wm)
{
  _wm = __wm;
  
  _sensor[0][0] = 0.0;
  _sensor[1][0] = 45.0;
  _sensor[2][0] = 90.0;
  _sensor[3][0] = 135.0;
  _sensor[4][0] = 180.0;
  _sensor[5][0] = 225.0;
  _sensor[6][0] = 270.0;
  _sensor[7][0] = 315.0;
    
  x_center = 0; y_center = 0;
  
  lengthLight = 20;
  

}

BasicPheromoneSensor::~BasicPheromoneSensor()
{

}

void BasicPheromoneSensor::update()
{
  _wm->_world->getAgent(_wm->_agentId)->getCoord(x_center, y_center);
  
  x_center += x_offset;
  y_center += y_offset;
    
  
  double orientation = _wm->_agentAbsoluteOrientation;
  for(int i = 0; i < 8; i++)
  {    
    
    double relative_orientation = orientation + _sensor[i][0];
    
    
    if (relative_orientation > 180) relative_orientation -=360;
    if (relative_orientation <= -180) relative_orientation += 360;
    
    _sensor[i][1] = relative_orientation;
    
    
    double co = cosine(relative_orientation);
    double si = sine(relative_orientation);
    
    _sensor[i][2] = round( x_center + co*lengthLight );
    _sensor[i][3] = round( y_center + si*lengthLight );
        
    _sensor[i][5] = getLightSensorValue(_sensor[i][2], _sensor[i][3]);
    
  }
 
  if (_wm->_agentId == 0)
  {
    
    //PRINT COORDINATES OF LIGHT SENSOR
//     std::cout << "         " << _sensor[0][2]<<","<<_sensor[0][3] << std::endl;
//     std::cout << "   " << _sensor[7][2]<<","<<_sensor[7][3] <<"     " << _sensor[1][2]<<","<<_sensor[1][3] << std::endl;
//     std::cout << "" << _sensor[6][2] <<","<<_sensor[6][3]<<"    "<<x_center<<"."<<y_center <<"    " << _sensor[2][2]<<","<<_sensor[2][3] << std::endl;
//     std::cout << "   " << _sensor[5][2]<<","<<_sensor[5][3] <<"     " << _sensor[3][2]<<","<<_sensor[3][3] << std::endl;
//     std::cout << "         " << _sensor[4][2] <<","<<_sensor[4][3]<< std::endl;
    
  
    //PRINT RELATIVE ANGLES OF SENSORS
//     std::cout << "         " << _sensor[0][1] << std::endl;
//     std::cout << "   " << _sensor[7][1] <<"     " << _sensor[1][1] << std::endl;
//     std::cout << "" << _sensor[6][1] <<"              " << _sensor[2][1] << std::endl;
//     std::cout << "   " << _sensor[5][1] <<"     " << _sensor[3][1] << std::endl;
//     std::cout << "         " << _sensor[4][1] << std::endl;

    
    //PRINT LIGHT SENSOR VALUES
//     std::cout << "         " << _sensor[0][5] << std::endl;
//     std::cout << "   " << _sensor[7][5] <<"     " << _sensor[1][5] << std::endl;
//     std::cout << "" << _sensor[6][5] <<"              " << _sensor[2][5] << std::endl;
//     std::cout << "   " << _sensor[5][5] <<"     " << _sensor[3][5] << std::endl;
//     std::cout << "         " << _sensor[4][5] << std::endl;
  }
 }
 


double BasicPheromoneSensor::getLightSensorValue(int x, int y)
{
  Uint32 pixel = getPixel32(gBackgroundImage, x, y);
  Uint8 r, g, b;
  SDL_GetRGB(pixel, gBackgroundImage->format, &r, &g, &b);
  if (r == 255 && g < 255 && b < 255) //Using green and blue channel to get alpha, since SDL_GetRGBA surface format doesn't have alpha component
    return (255.0 - (double)b)/255.0; //return normalized value
  
  return 0;
}


int BasicPheromoneSensor::getIndexOfMaxLeftLightSensor()
{
  int maxIndex = -1;
  double maxVal = 0;
  
  for (int i = 6; i <= 7; i++)
  {
    if (_sensor[i][5] > maxVal)
    {
      maxVal = _sensor[i][5];
      maxIndex = i;
    } 
  }
  
  return maxIndex;
}

int BasicPheromoneSensor::getIndexOfMaxRightLightSensor()
{
  int maxIndex = -1;
  double maxVal = 0;
  
  for (int i = 1; i <= 2; i++)
  {
    if (_sensor[i][5] > maxVal)
    {
      maxVal = _sensor[i][5];
      maxIndex = i;
    }
  }
  
  return maxIndex;
}


double BasicPheromoneSensor::getRightLightMax()
{
  double maxVal = 0;
  
  for (int i = 1; i <= 2; i++)
  {
    if (_sensor[i][5] > maxVal)
    {
      maxVal = _sensor[i][5];
    }
  }
  
  return maxVal;
}

double BasicPheromoneSensor::getLeftLightMax()
{
  double maxVal = 0;
  
  for (int i = 6; i <= 7; i++)
  {
    if (_sensor[i][5] > maxVal)
    {
      maxVal = _sensor[i][5];
    } 
  }
  
  return maxVal;
}


double BasicPheromoneSensor::sine(double theta)
{
  
  return sin(theta*pi/180);
}

double BasicPheromoneSensor::cosine(double theta)
{
  return cos(theta*pi/180);
  
}
 
 
 