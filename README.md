## KasperskyOS_Hackaton2022
Repository for KasperskyOS hackaton by Kaspersky Laboratory based at MAI university.  

## Repository  
### Quick Access
[Used docker tools](Qualification/Task1) - folder, containing used dockerfile and necessary bash scripts    
[Used bash scripts](MainPart/mqtt_gpio) - folder, containing used bash scripts, developed for comfort development   

[Qualification Task 1](Qualification/Task1)   
[Qualification Task 2](Qualification/Task2)   
[Qualification Task 3](Qualification/Task3)  
[Qualification Task 4](Qualification/Task4)  
  
[Latest working version](MainPart/monolith_client) - folder, containing tested, dirty, but working code  
[Latest improved code version](MainPart/mqtt_gpio) - folder, containing clean, modern code, but what sometimes causes an unknown error on Raspberry Pi  

### Repository structure  
```bash
.  
├── Qualification/                      # folder for hackathon qualification stage  
|    ├── Task1/  
|    ├── Task2/  
|    ├── Task3/  
|    ├── Task4/  
|    ├── Qualification_Tasks.pdf        # full description of the tasks  
|    └── README.md                      # short description of the tasks  
|  
├── MainPart/  
|    ├── monolith_client/               # the latest working version  
|    ├── mqtt_gpio/                     # modern code-style, but unstabely working on robot example  
|    ├── Convert_coordinates.cpp        # cpp code for convertion for screen to floor coordinates  
|    └── mosquitto.conf                 # basic config for mqtt broker  
|
├── entities/                           # tries of the IPC implementation  
|  
├── .gitignore  
├── antivirus.png  
├── LICENSE.md  
└── README.md                           # you are here  
```

## Current jobs
 - [x] Edit license for hackaton
 - [x] Qualification Task 1
 - [x] Qualification Task 2
 - [x] Qualification Task 3
 - [ ] Qualification Task 4
 - [x] Main Task  

## Authors
Tsaturyan Konstantin, Alexey Volchenkov, Filatov Nikita, Alexander Timoshenko  
Moscow Aviation Institute  
Moscow, 2022  
