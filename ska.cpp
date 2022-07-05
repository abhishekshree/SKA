#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

// SKA data entity
class Data {
private:
  string name;
  int size;
  int rate;
  vector<int> payload;

public:
  Data(string name, int size, int rate) {
    this->name = name;
    this->size = size;
    this->rate = rate;
  }

  string getName() { return name; }

  int getSize() { return size; }

  int getRate() { return rate; }

  vector<int> getPayload() { return payload; }

  void setPayload(vector<int> payload) { this->payload = payload; }
};

// sender or receiver nodes
class Node {
private:
  string uuid;
  bool type; // 0 sender
  int capacity;
  int max_rate;
  vector<Data> data;

public:
  string get_uuid() {
    static random_device dev;
    static mt19937 rng(dev());

    uniform_int_distribution<int> dist(0, 15);

    const char *v = "0123456789abcdef";
    const bool dash[] = {0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0};

    string res;
    for (int i = 0; i < 16; i++) {
      if (dash[i])
        res += "-";
      res += v[dist(rng)];
      res += v[dist(rng)];
    }
    return res;
  }

  Node(bool type, int capacity, int max_rate) {
    this->uuid = get_uuid();
    this->type = type;
    this->capacity = capacity;
    this->max_rate = max_rate;
  }

  bool getType() { return type; }

  int getCapacity() { return capacity; }

  int getMaxRate() { return max_rate; }

  vector<Data> getData() { return data; }

  int size() { return data.size(); }

  void addData(vector<Data> data) {
    if (this->size() >= this->capacity) {
      return;
    }

    for (int i = 0; i < data.size(); i++) {
      this->data.push_back(data[i]);
    }
  }

  void removeData(int index) {
    if (index >= this->size()) {
      return;
    }
    
    this->data.erase(this->data.begin() + index);
  }
};

// To simulate data transfer between nodes as Discrete Events
class Simulator {
private:
  int num_sender;
  int num_receiver;
  vector<Node> senders;
  vector<Node> receivers;
  int rate;
  int cur_transfers;
  int max_transfers;
  vector<string> log;

  void addLog(string message) { log.push_back(message); }

public:
  Simulator(int num_sender, int num_receiver, int rate, int max_transfers) {
    this->num_sender = num_sender;
    this->num_receiver = num_receiver;
    this->rate = rate;
    this->max_transfers = max_transfers;
    this->cur_transfers = 0;
  }

  void addSender(Node sender) {
    if (senders.size() >= num_sender) {
      addLog("[ERROR]: too many senders");
      return;
    }
    senders.push_back(sender);
  }

  void addReceiver(Node receiver) {
    if (receivers.size() >= num_receiver) {
      addLog("[ERROR]: too many receivers");
      return;
    }
    receivers.push_back(receiver);
  }

  // one sender sending to all receivers
  void sendOneToAll(int sender_id) {
    Node sender = senders[sender_id];
    int r = rate;
    int num_receiver = receivers.size();

    int counter = 0;

    // if sender cannot send at rate r, use the max rate of sender
    if (r > sender.getMaxRate()) {
      r = sender.getMaxRate();
    }

    // copy data from sender to receiver r at a time
    while (counter < num_receiver) {
      if (this->cur_transfers >= this->max_transfers) {
        addLog("[ERROR]: too many transfers");
        break;
      }

      for (int i = 0; i < r; i++) {
        if (counter >= num_receiver) {
          break;
        }

        if (receivers[counter].size() >= receivers[counter].getCapacity()) {
          counter++;
          addLog("[ERROR]: receiver " + (receivers[counter].get_uuid()) +
              " is full");
          continue;
        }

        receivers[counter].addData(sender.getData());
        counter++;
        this->cur_transfers++;

        addLog("[INFO]: " + sender.get_uuid() + " sent to " +
            receivers[counter].get_uuid() + " in batch " +
            to_string((int)((counter + 1) / r)));
      }
    }
  }

  // Many senders sending to all receiver
  void sendManyToAll(vector<int> sender_ids) {
    int num_receiver = receivers.size();
    int r = rate;
    int counter = 0;

    if (r > max_transfers) {
      r = max_transfers;
    }

    for (int sid : sender_ids) {
      this->sendOneToAll(sid);
    }
  }

  void printLog() {
    for (string message : log) {
      cout << message << endl;
    }
  }

  ~Simulator() {
    senders.clear();
    receivers.clear();
    log.clear();
  }
};

int main() {
  Simulator simulator(2, 2, 1, 10);
  simulator.addSender(Node(0, 1, 1));
  simulator.addSender(Node(0, 1, 1));
  simulator.addReceiver(Node(1, 1, 1));
  simulator.addReceiver(Node(1, 1, 1));
  simulator.sendManyToAll({0, 1});
  simulator.printLog();

  cout << "==============================" << endl;

  Simulator simulator2(5, 10, 1, 10);
  for (int i = 0; i < 5; i++) {
    simulator2.addSender(Node(0, 1, 1));
  }
  for (int i = 0; i < 10; i++) {
    simulator2.addReceiver(Node(1, 1, 1));
  }
  simulator2.sendManyToAll({0, 1, 2, 3, 4});
  simulator2.printLog();

  return 0;
}
