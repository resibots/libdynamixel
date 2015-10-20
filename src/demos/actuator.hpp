#include <dynamixel/dynamixel.hpp>

typedef enum {clockwise, counterclockwise} t_direction;

/**
    Actuator generates a vector of poses for an actuator, using the parameters
    given to its constructor. Then, the resulting data is available in different
    forms, through a few methods.
 */
class Actuator {

protected :

    std::vector<unsigned int> _discretePositions;

    unsigned int _index;

    dynamixel::byte_t _id;

    int _initialPosition;

    float _amplitude;

    float _angularFrequency;

    float _phase;

    unsigned int _samplingFrequency;

    t_direction _direction;

public :
    Actuator(dynamixel::byte_t id,
             int initialPosition = 512,
             float amplitude = 60.0,
             float angularFrequency = 1.0,
             float phase = 0.0,
             unsigned int samplingFrequency = 10,
             t_direction direction = clockwise);

    void sample();

    unsigned int nextPosition();

    void printDiscretePositions();

    void printArrayDefinition();

    void printFunction();



    dynamixel::byte_t getId();

    int getInitialPosition();

    float getAmplitude();

    float getAngularFrequency();

    float getPhase();

    unsigned int getSamplingFrequency();

    t_direction getDirection();

    const std::vector<unsigned int>& getDiscretePositions();
};

