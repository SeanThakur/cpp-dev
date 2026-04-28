#include <iostream>

class Vector2
{
public:
    float x;
    float y;
    Vector2(float x = 0.0f, float y = 0.0f)
    {
        // variable shadowing, we can use this-> to refer to the class member variables
        this->x = x;
        this->y = y;
    }

    // operator overload for addition of two Vector2 objects
    // we can also overload other operators like -, *, /, etc. as needed
    // not using &operator because we want to return a new Vector2 object, not modify the existing one
    Vector2 operator+(const Vector2 &other) const
    {
        return Vector2(this->x + other.x, this->y + other.y);
    }
};

int main()
{
    Vector2 position(10.5f, 20.0f);
    Vector2 velocity(2.0f, 5.0f);

    Vector2 newPosition = position + velocity;
    std::cout << "New Position: (" << newPosition.x << ", " << newPosition.y << ")\n";
    return 0;
}