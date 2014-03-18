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


#include "BasicPheromone/include/Particle.h"


Particle::Particle()
{
  e = 0;
  n = 0;
  w = 0;
  s = 0;
  intensity = 0;
  
  int _lifetime = 500;
  int _interval = 20;
  
  evaporationFactor = exp((log(0.5)/_lifetime )*_interval);
 
}

Particle::~Particle()
{
}


void Particle::swap()
{
  if (intensity == 0)
  {
    e = 0;
    n = 0;
    w = 0;
    s = 0;
  }
  else
  {
  e = eNext;
  n = nNext;
  w = wNext;
  s = sNext;
  }
  
  eNext = 0;
  nNext = 0;
  wNext = 0;
  sNext = 0;
}

int Particle::getIntensity()
{
//   intensity *= 0.99;


  return intensity;
}

void Particle::setIntensity(int _intensity)
{
  intensity = _intensity;
}

void Particle::evaporate()
{
  double a = (double)intensity/255.0;
  a *= evaporationFactor;
  intensity = 255*a;
}

void Particle::mixIntensities(Particle *p)
{
  int dividend = 0;
  dividend += e ? 1 : 0;
  dividend += n ? 1 : 0;
  dividend += w ? 1 : 0;
  dividend += s ? 1 : 0;
  
  if (dividend == 0)
    return;
  
  int intens = p->getIntensity();
  int newIntens = (intens + intensity)/dividend;
  p->setIntensity(newIntens);
  intensity = newIntens;
}

