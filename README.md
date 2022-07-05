# SKA

The C++ file consists of three classes:
- `Data`: Example SKA data entity with parameters and data.
- `Node`: A node refers to the entity which contains data to be processed and transmitted.
- `Simulator`: The simulator is the main class which controls the flow of data across senders and receivers.

---

The simulator class consists of the simulation logic. 
1. One sender node sending data to all other receiver nodes.
2. "N" sender node sending data to all other receiver nodes.

Instructions to run the simulation:
```bash
g++ ska.cpp -o ska && ./ska
```

---

Examples:
1. 2 senders, 2 receivers.
```cpp
Simulator simulator(2, 2, 1, 10);
simulator.addSender(Node(0, 1, 1));
simulator.addSender(Node(0, 1, 1));
simulator.addReceiver(Node(1, 1, 1));
simulator.addReceiver(Node(1, 1, 1));
simulator.sendManyToAll({0, 1});
simulator.printLog();
```

Results of the simulation:
```
[INFO]: f6086c90-0698-3f85-5d6d-1a251079a41f sent to bcf3baac-5a8d-efcc-13a9-7baaab0a6ebb in batch 2
[INFO]: 1e34e08d-a6ef-5675-25a1-b989c86ae18e sent to fc623fe4-5c31-e32a-7789-5e4f7df2cd49 in batch 3
[INFO]: 639d8e8c-9876-e85c-ba19-955d7d53b496 sent to 1c136b96-6ac2-fc3f-baa0-51a4f81c8cc3 in batch 2
[INFO]: d1a747c2-ac00-8b69-99f1-415e0aa725fd sent to 63c2c518-acce-5b57-fd13-eb61b849bf5a in batch 3
```

2. 5 senders, 10 receivers.
```cpp
Simulator simulator2(5, 10, 1, 10);
for (int i = 0; i < 5; i++) {
    simulator2.addSender(Node(0, 1, 1));
}
for (int i = 0; i < 10; i++) {
    simulator2.addReceiver(Node(1, 1, 1));
}
simulator2.sendManyToAll({0, 1, 2, 3, 4});
simulator2.printLog();
```

Results of the simulation:
```
[INFO]: aafc960e-cfe3-4011-9562-5e0b190be05d sent to 27521921-318f-6e9a-9a17-4f36dc046cea in batch 2
[INFO]: 25f3c5d2-c607-d70a-6ef6-acd6efacdeb0 sent to a5bb339b-c99f-e25b-16bb-fa6d9eec26a6 in batch 3
[INFO]: 095b71eb-0a99-6a41-cdf1-a1bea2d5c114 sent to f064d586-0547-942a-8690-8d6d67e7b7bf in batch 4
[INFO]: 8e2bf463-8e26-d061-ed4c-0ea903734513 sent to 4ca5eb75-b823-4feb-6e78-abd73f2a5934 in batch 5
[INFO]: 4deec805-f18f-b6fd-6f4b-40d928c9d3a4 sent to e4583fc8-75ec-3373-92ee-16e3dc607657 in batch 6
[INFO]: 2ebb0607-3212-c4e9-2a8b-8bec6874ed55 sent to 5bd990c3-dc2b-6053-56f0-8c5b90b932b5 in batch 7
[INFO]: 36260399-f0d4-c6e9-bf2a-bb3f14d4dd73 sent to 04e3ba01-c8e9-92a1-e90d-7070e64a19c2 in batch 8
[INFO]: 8821a33c-c29a-490c-280c-2f2d826fbfd3 sent to b6dfca7f-ae69-fa1c-d8ca-cba94fd0d597 in batch 9
[INFO]: edaac989-a138-e328-702f-bb29cb26ecd0 sent to dc8a626f-828a-3bb1-8caf-f4a3331cad1f in batch 10
[INFO]: bec0f6eb-06e0-878e-99bf-2bb22d62cda3 sent to 32235192-51b0-b76e-8698-616f0633b9e5 in batch 11
[ERROR]: too many transfers
[ERROR]: too many transfers
[ERROR]: too many transfers
[ERROR]: too many transfers
```

The senders try to send all 15 data packets to the receivers. But with the maximum transfers being limited to 10, the senders will stop sending after 10 transfers.
