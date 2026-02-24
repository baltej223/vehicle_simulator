#include<iostream>
#include<vector>
#include<string>
#include<functional>

template <typename T>
class Receiver{
    public:
    T DataReceived;
    
    Receiver(){

    }
    void SendHook(T data){
        this->DataReceived = data;
    }

    T GetData(){
        return this->DataReceived; 
    }
};

template <typename T>
class Brodcaster{
    public:
    T DataToBroadcast;
    std::vector<Receiver> Subscribers;
    int TotalSubscribers = 0;

    void SetSubscriber(Receiver rec){
        this->Subscribers.push_back(rec);
        this->TotalSubscribers+=1;
    }

    bool Broadcast(T data){
        this->DataToBroadcast = data; // Redundant but archival
        for (int subcriber : Subscribers){
            subcriber.SendHook(data);
        }
        return true;
    }
};

int main(){
    Brodcaster<int> b1;

    Receiver r1;
    b1.SetSubscriber(r1);
}
