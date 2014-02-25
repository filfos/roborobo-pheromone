/*
    Copyright 2014 <copyright holder> <email>

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


#ifndef COORDINATE_H
#define COORDINATE_H

class Coordinate
{
  public:
    Coordinate(int x, int y);
    ~Coordinate();
    
    int x();
    int y();
    
    void update(int x, int y);
    
  protected:
     int _x;
     int _y;
  
    
  
  
  
  
};

#endif // COORDINATE_H
