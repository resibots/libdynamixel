#include <math.h>
#include <iostream>
#include "actuator.hpp"

Actuator::Actuator(dynamixel::byte_t id,
                   int initialPosition,
                   float amplitude,
                   float angularFrequency,
                   float phase,
                   unsigned int samplingFrequency,
                   t_direction direction)
                   :
                   _id(id),
                   _initialPosition(initialPosition),
                   _amplitude(amplitude),
                   _angularFrequency(angularFrequency),
                   _phase(phase),
                   _direction(direction) {
    _samplingFrequency = samplingFrequency > 0 ? samplingFrequency : 1;
    _index = 0;
    sample();
}

void Actuator::sample() {
    _discretePositions.clear();

    float samplingInterval = (double) 1.0 / ((double) _samplingFrequency * (double) _angularFrequency);
    float t = 0;
    for(unsigned int sample = 0 ; sample < _samplingFrequency ; sample++)
    {
        // Calculate new positions
        int position = _initialPosition
                       + _amplitude
                       * (_direction == clockwise ? -1 : 1)
                       * sin(t * 2 * M_PI * _angularFrequency 
                             + _phase);      // a * cos(w * t + d)

        _discretePositions.push_back(position);

        t += samplingInterval;
    }
}

unsigned int Actuator::nextPosition() {
    _index = (_index + 1) % _samplingFrequency;
    return _discretePositions[_index];
}

// Can help plotting discetes positions with gnuplot, octave, R, ...
void Actuator::printDiscretePositions() {
    std::cout << "Actuator #" << (unsigned int) _id << " :" << std::endl;

    float t = 0;
    float samplingInterval = (double) 1.0 / ((double) _samplingFrequency * (double) _angularFrequency);

    for(unsigned int i = 0 ; i < _discretePositions.size() ; i++) {
        std::cout << t << "\t" << (unsigned int) _discretePositions[i] << std::endl;
        t += samplingInterval;
    }

    std::cout << std::endl;
}

// Can help to "export" discetes positions into another C/C++ module (hylos, ...)
void Actuator::printArrayDefinition() {
    std::cout << "unsigned int actuator_" << (unsigned int) _id << "[" << _discretePositions.size() << "] = {";
    for(unsigned int i = 0 ; i < _discretePositions.size() ; i++)
        std::cout << (unsigned int) _discretePositions[i] << (i == _discretePositions.size()-1 ? "};" : ", ");
    std::cout << std::endl;
}

// Can help plotting expected positions with gnuplot, octave, R, ...
void Actuator::printFunction() {
    std::cout << _initialPosition
    << " + " << _amplitude
    << " * " << (_direction == clockwise ? -1 : 1)
    << " * sin(x * 2 * pi * " << _angularFrequency << " + " << _phase << ")"
    << std::endl;
}



dynamixel::byte_t Actuator::getId() {
    return _id;
}

int Actuator::getInitialPosition() {
    return _initialPosition;
}

float Actuator::getAmplitude() {
    return _amplitude;
}

float Actuator::getAngularFrequency() {
    return _angularFrequency;
}

float Actuator::getPhase() {
    return _phase;
}

unsigned int Actuator::getSamplingFrequency() {
    return _samplingFrequency;
}

t_direction Actuator::getDirection() {
    return _direction;
}

const std::vector<unsigned int>& Actuator::getDiscretePositions() {
    return _discretePositions;
}
