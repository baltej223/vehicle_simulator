#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <cstdlib>

#define INFINITY (unsigned)!((int)0)
#define MINIMUM_DISTANCE_BETWEEN_CARS 100
/*
NOTE:
- I was thinking for multiple cars to run at the sametime, 
they will have there own algorithem using which the car can take autonmous decisions.

- For that I was thinking that a state like mechanism has to be made, and I am really thinking about it,
how can something like use state can be made

_ Unlike state like mechanism which exists in react.js, I want two class to communicate between each other.
- If one object got to know that the other class has changes and, then the claass just gets the changes from the other class 
- Just other object gets to know that there has been some changes which it can get.
- But in this kind of mechanism, there is a flaw, which is, We should be able to simulate as much cars as possible, not just two.
- So i want Some broadcasting and subscriber mechanism.
- Nah, I don't need any broadcasting mechanics, basically, I need to make an array of all the vehicals,
in the road itself.
- And let the vehicals read from this array repeatedely.
*/


class PhysicalObject{

};

class Vehicle;

class Road : public PhysicalObject{
    public: 
    int width;
    bool isSimpleRoad;
    std::vector<float> road_vector;
    int precision_distance;
    int length; // The total length of the road
    int totalLanes; // between [1, totalLanes], 1: means the leftest lane;
    // Here comes the  array of the vehicals, running on the road.
    std::vector<Vehicle*> onRoadVehicals;

    private:
    int minWidth = 10;

    public:
    Road(int width, int length, int totalLanes) { // lanes
        if (!(width>=minWidth)) {
          throw std::invalid_argument("Width too smoll");
        }
        else {
          this->width = width;
          this->length = length;
          this->totalLanes = totalLanes;
          }
      }

    bool CheckRoadLength(int precision_distance, std::vector<float> road_vector){
        // std::cout << length << "\n";
        // std::cout << width << "\n";
        // std::cout << road_vector.size();
        if (road_vector.size()*precision_distance == this->length){
            // std::cout << (road_vector.size()*precision_distance == this->length) ? "true" : "flase";
            return true;
        }
        else {
            return false;
        }
    }

    void simpleRoad(int precision_distance, std::vector<float> road_vector){
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
        if (!(CheckRoadLength(precision_distance, road_vector))) {
          throw std::invalid_argument("You have to provide appropreate length of the the road vector,\n[Note]: Current roads vectors if added, doesn't match the road length provided in defination\n");
        } else {
          this->isSimpleRoad=true;
          this->road_vector = road_vector;
          this->precision_distance = precision_distance;
        }
    }
};


class Vehicle{
  public:
  int road_width;
  int distance_completed=0;
  int speed = 0; // meters per seconds.
  Road *road;
  int current_lane = 1;
  std::string name;
  static int totalCars;

  Vehicle(Road* rd, std::string name = "default", int distance = 0): road(rd) {
    totalCars =totalCars + 1;
  
    this->road_width = rd->width;
    this->road = rd;
    // Now I need to put this object's pointer in the vectors<Vehicle> of the road.
    this->road->onRoadVehicals.push_back(this);
    this->distance_completed = distance;
    
    if (name == "default") {
        this->name = "Vehicle " + totalCars;
    } else {
      this->name = name;
    }
  }
  int Run(int lane, int speed) {
    this->speed = speed;
    // First I need to put the lane in the current_lane variable, and also check before putting,
    // if the input from the user is not greater that the number of lanes of the road.
    if (!(lane > 0 && lane <= this->road->totalLanes)){ 
      throw std::invalid_argument("Bad parameters!");
    } else {
      this->current_lane = lane;
    }
      // It records the position of the car from left side of the road. 
      // Initially start from some random x from road's left side.
      // There will be loop, in every loop, the car will cover some predefined distance.     : I changed this idea
      //
      // Now there will be lanes, and the car will be changing lanes. The lanes will be predefined. 

    // Either I need to know the distance that will be covered, either I need to know the speed of the car, 
    // only then I can find the time that need to be waited for each of the car.
    // And It makes more scence, for the car to get the distance from the road, the road.
    // But a running car shoudn't need to know the distance, that it need to travel
    // so lets go with the car knowing its speed;
    // Now I need to determine, How much I need this thread to wait.
    // The point is that the thread waits, for each of the car, till the car covers the precison_distance, 
    // Now I need to calculate with that speed, How fast the car will calculate the precison_distance
  

    // std::cout << "Time required for covering the precision_distance for the car is : "<< time_covering_precision_distance;
      
    for (int i : road->road_vector){
      // I specifically put this in the loop, so that when I change the speed, then the time waiting for the car also changes.

      double time_covering_precision_distance = (double) this->road->precision_distance /  (double)  speed;
      
          observeAndDrive();
          // std::cout << i << "\n";
          this->distance_completed += speed * time_covering_precision_distance;
          std::cout << "["+this->name+"] " << "Distance Completed:" << distance_completed << std::endl;
          std::cout << "Speed: " << speed << std::endl;
          std::this_thread::sleep_for(std::chrono::duration<double>(time_covering_precision_distance)); 
    }
      return distance_completed;
  }
 
  void observeAndDrive() { // Change Lanes and speed
      // Here comes the actual algorithem;
    std::vector<std::pair<int, int>> distances;
    for (Vehicle *other_car : road->onRoadVehicals ) {
      int distance_diff = other_car->distance_completed - this->distance_completed; // -ve means behind, and +ve means ahead 
      int lane_diff = other_car->current_lane - this->current_lane; // to the left: +ve | to the right: -ve
      std::pair<int, int> p = {distance_diff, lane_diff};
      distances.push_back(p);
    }

    // if there is something ahead (100 meter) going slow, then change lane to left, 
    // if left is busy, for example, something is comming fast from beghind, or something is going just with you.
    // then go to right, if right not possible, then slow down.
    // if possible then slow down;
    // I guess thats all, I need to implement;

    // so first I need to find whats going ahead of me.
    int minDistance = INFINITY;
    for (int i = 0 ; i < distances.size(); i++) {
      std::pair<int, int> distance = distances[i];
      if (!(distance.second == this->current_lane)) {
        continue;
      }
      if (distance.second < minDistance) {
        minDistance = distance.second;
      }
    }
    if (!(minDistance < MINIMUM_DISTANCE_BETWEEN_CARS)) return; // do nothing, 
                                                                // if all the cars are very far apart
    if (this->current_lane > 1) {
      // Now I need to check if there is any car of the left lane 
      // which is comming fast (has more speed than me), and is near than const MINIMUM_DISTANCE_BETWEEN_CARS,
      std::vector<Vehicle*> onLeftLane = this->onLane(current_lane-1);
      for (Vehicle* v:  onLeftLane) {
        if (v->speed > this->speed && this->mod(v->distance_completed - this->distance_completed) < MINIMUM_DISTANCE_BETWEEN_CARS) {
          if (this->current_lane < this->road->totalLanes) {
            std::vector<Vehicle*> onRightLane = this->onLane(current_lane + 1);
            for (Vehicle* v_r : onRightLane) {
              if (v_r->speed > this->speed && this->mod(v->distance_completed - this->distance_completed) < MINIMUM_DISTANCE_BETWEEN_CARS) {
                // slow 
                this->speed = this->speed - 10;
              }
            }
          } else {
            // slow 
            this->speed = this->speed -10;

          }
        } else {
          this->current_lane = current_lane - 1; 
        }
      }
    } else if (current_lane == 1){
      // slow 
      this->speed = this->speed - 10;
    }
  }
  private : 
  std::vector<Vehicle*> onLane(int lane){
    std::vector<Vehicle*> toReturn;
    for (Vehicle *v : this->road->onRoadVehicals) {
      if (v->current_lane == lane) {
        toReturn.push_back(v);
      }
    }
    return toReturn;
  }
  int mod(int num) {
    return num < 0 ? num * -1 : num;  
  }
};

  int Vehicle::totalCars = 0;

int main(){
    Road NewRd(200, 500, 4);
    std::vector<float> RdAngle = {90, 90, 0, 0, -90}; 
    NewRd.simpleRoad(100, RdAngle);

    Vehicle Car1(&NewRd, "Car 1", 100);
    Vehicle Car2(&NewRd, "Car 2", 0);
    // std::cout << Car.Run(1, 200);

    std::thread t1(&Vehicle::Run, &Car1, 1, 200);
    std::thread t2(&Vehicle::Run, &Car2, 1, 400);
    
    t1.join();
    t2.join();
}
