#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>

/*
NOTE:
- I was thinking for multiple cars to run at the sametime, 
they will have there own algorithem using which the car can take autonmous decisions.

- For that I was thinking that a state like mechanism has to be made, and I am really thinking about it,
how can something like use state can be made

*/

class PhysicalObject{

};

class Road : public PhysicalObject{
    
    public: 
    int width;
    bool isSimpleRoad;
    std::vector<float> road_vector;
    int precision_distance;
    int length; // The total length of the road

    private:
    int minWidth = 10;

    public:
    Road(int width, int length){
        if (width>=minWidth){
            this->width = width;
            this->length = length;
        }
        else {
            std::cout << "The width of the roads should be more that "<<minWidth<<"\n";
            // Atleast at a time, two cars should be able to fit in the road
        }
    }

    bool CheckRoadLength(int precision_distance, std::vector<float> road_vector){
        // std::cout << length << "\n";
        // std::cout << width << "\n";
        // std::cout << road_vector.size();
        if (road_vector.size()*precision_distance == length){
            // std::cout << (road_vector.size()*precision_distance == this->length) ? "true" : "flase";
            return true;
        }
        else {
            return false;
        }
    }

    void Error(std::string error){
        std::cout << "[ERROR]\n";
        std::cout << error;
    }

    void simpleRoad(int precision_distance, std::vector<float> road_vector){
        this->isSimpleRoad=true;
        // Lets say precision_distance is x;
        // So basically you give the precision_distance,
        // Which is, after each of this distance you will have to tell in which direction you want to go
        // Right?!
        // I can define like {21, 23, 32} 
        // Which means start from 0 distance, cover x distance in 21 degree angle.
        // then if the roads is towards, 
        // say 'm', then from m as your base, 
        // set your angle to 23 and then go cover next d distance.
        // +23 means 23deg to the left and -23deg means toward to right

        // Check if the roadVector.size() * precison_distance == this->length
        if (CheckRoadLength(precision_distance, road_vector)){
            this->road_vector = road_vector;
            this->precision_distance = precision_distance;
        } else {
            Error("You have to provide appropreate length of the the road vector,\n[Note]: Current roads vectors if added, doesn't match the road length provided in defination\n");
        }
        // Actuall coding will be done for the car, not the road.
    }

};


class Vehicle{
    public:
    int road_width;
    int distance_completed=0;
    int speed;
    Road road;

    Vehicle(Road rd, int speed): road(rd){
        this->road_width = rd.width;
        this->speed = speed;
        this->road = rd;
        }
        
        int Run(){
            // It records the position of the car from left side of the road.
            // Initially start from some random x from road's left side.
            // There will be loop, in every loop, the car will cover some predefined distance.
            for (int i : road.road_vector){
                std::cout<<i<<"\n";
                this->distance_completed += speed;
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            return distance_completed;
        }
    };

int main(){
    Road NewRd(200, 500);
    std::vector<float> RdAngle = {90, 90, 0, 0, -90}; 
    NewRd.simpleRoad(100, RdAngle);

    Vehicle Car(NewRd, 200);
    std::cout << Car.Run();
}