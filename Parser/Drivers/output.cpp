#include<iostream>
#include <fstream>
#include <sstream>

using namespace std;
#include "master.h"

int main(int argc,char* argv[]) {
    //Parsing of Schedule File

    ifstream scheduleFile;
    scheduleFile.open(argv[1]);
    string line;
    getline(scheduleFile, line);

    while (getline(scheduleFile, line)) {

        stringstream inputString(line);
        //ScheduleID,CarrierCD,FlightNumber, AircraftType, AircraftTailNumber,DepartureTime,ArrivalTime,StartDate,EndDate,FlightPattern,Status,DepartureDates
        string ScheduleID;
        char CarrierCD[CARRIER_CODE_LENGTH];
        int FlightNum;

        pair<string,string> EquipmentNo;

        string Src;
        string Dest;

        Time DepartureTime;
        Time ArrivalTime;
        Date StartDate;
        Date EndDate;

        Statuses Status;

        string FlightPattern;
        string tempString;

        getline(inputString, ScheduleID, ',');
        int uuid=scheduleUuidGenerator.getID(ScheduleID);


        getline(inputString,tempString, ',');
        for(int i=0;i<CARRIER_CODE_LENGTH;i++) CarrierCD[i]= tempString[i];

        getline(inputString,tempString, ',');
        FlightNum = atoi(tempString.c_str());

        getline(inputString, EquipmentNo.first, ',');
        getline(inputString, EquipmentNo.second, ',');

        getline(inputString, tempString , ',');
        assert(tempString.size()==CITY_CODE_LENGTH);
        Src=tempString;

        getline(inputString, tempString, ',');
        assert(tempString.size()==CITY_CODE_LENGTH);
        Dest=tempString;

        flightNumberMap[FlightNum]={Src,Dest};
        cityToFlightNumberMap[make_pair(Src,Dest)]=FlightNum;

        getline(inputString, tempString, ',');
        DepartureTime=Time(tempString);

        getline(inputString, tempString, ',');
        ArrivalTime=Time(tempString);

        getline(inputString, tempString, ',');
        StartDate=Date(tempString);

        getline(inputString, tempString, ',');
        EndDate=Date(tempString);

        getline(inputString, tempString, ',');
        Status = static_cast<Statuses> (getStatusCode(tempString));

        getline(inputString, tempString, ',');
        getline(inputString, tempString, ',');
        getline(inputString, tempString, ',');
        getline(inputString, tempString, ',');
        getline(inputString, tempString, ',');
        getline(inputString, tempString, ',');
        getline(inputString, tempString, ',');
        getline(inputString, tempString, ',');

        getline(inputString, FlightPattern, ',');


        Schedule* S = new Schedule(uuid,CarrierCD,EquipmentNo,DepartureTime,ArrivalTime,StartDate,EndDate,FlightPattern,FlightNum,Status);

        scheduleMap[uuid]=S;
    }








    //Parsing of Inventory File

    ifstream inventoryFile;
    inventoryFile.open(argv[2]);
    getline(inventoryFile, line);

    bool flag=true;

    while (getline(inventoryFile, line)) {
        stringstream inputString(line);

        string InventoryID;
        string ScheduleID;
        Date DepartureDate, ArrivalDate;
        int TotalCapacity, TotalInventory;
        int FCTotalCapacity, FCTotalInventory;
        int BCTotalCapacity, BCTotalInventory;
        int PCTotalCapacity, PCTotalInventory;
        int ECTotalCapacity, ECTotalInventory;

        string tempString;

        getline(inputString, InventoryID, ',');
        int uuid = inventoryUuidGenerator.getID(InventoryID);

        getline(inputString, ScheduleID, ',');

        int s_id = scheduleUuidGenerator.getID(ScheduleID);

        inventoryToScheduleMap[uuid]=s_id;

        if(scheduleMap[s_id]->Status == 2) CancelledFlights.insert(uuid);

        scheduleMap[s_id]->DepartureDates.push_back(uuid);

        getline(inputString, tempString, ',');

        getline(inputString, tempString, ',');

        getline(inputString, tempString, ',');
        DepartureDate = Date(tempString);


        getline(inputString, tempString, ',');
        ArrivalDate = Date(tempString);

        getline(inputString, tempString, ',');

        getline(inputString, tempString, ',');

        getline(inputString, tempString, ','); TotalCapacity = atoi(tempString.c_str());
        getline(inputString, tempString, ','); TotalInventory = atoi(tempString.c_str());
        getline(inputString, tempString, ',');

        getline(inputString, tempString, ',');

        getline(inputString, tempString, ',');

        getline(inputString, tempString, ','); FCTotalCapacity = atoi(tempString.c_str());
        getline(inputString, tempString, ','); BCTotalCapacity= atoi(tempString.c_str());
        getline(inputString, tempString, ','); PCTotalCapacity = atoi(tempString.c_str());
        getline(inputString, tempString, ','); ECTotalCapacity = atoi(tempString.c_str());

        getline(inputString, tempString, ','); FCTotalInventory = atoi(tempString.c_str());
        getline(inputString, tempString, ',');
        getline(inputString, tempString, ',');
        getline(inputString, tempString, ',');


        getline(inputString, tempString, ','); BCTotalInventory = atoi(tempString.c_str());
        getline(inputString, tempString, ',');
        getline(inputString, tempString, ',');
        getline(inputString, tempString, ',');

        getline(inputString, tempString, ','); PCTotalInventory = atoi(tempString.c_str());
        getline(inputString, tempString, ',');
        getline(inputString, tempString, ',');
        getline(inputString, tempString, ',');

        getline(inputString, tempString, ','); ECTotalInventory = atoi(tempString.c_str());
        getline(inputString, tempString, ',');
        getline(inputString, tempString, ',');
        getline(inputString, tempString, ',');

        while(flag){
            for (auto &ch: {"FC", "BC", "PC", "EC"}) {
                getline(inputString, tempString, '"');

                getline(inputString, tempString, '"');
                for (auto &c: tempString) if (c >= 'A' and c <= 'Z') cabinToClassMap[c] = ch;

            }
            flag=false;
        }

        line = "";
        Inventory* I= new Inventory(uuid, DepartureDate, ArrivalDate, TotalCapacity, TotalInventory,0, FCTotalCapacity, FCTotalInventory,0, BCTotalCapacity, BCTotalInventory,0, PCTotalCapacity, PCTotalInventory,0, ECTotalCapacity, ECTotalInventory, 0);

        inventoryMap[uuid]=I;


    }








    // Parsing of Passenger Booking File

    ifstream bookingFile;
    bookingFile.open(argv[3]);
    line = "";
    getline(bookingFile, line); line.clear();

    int uuid=0;
    int prev_seg_seq = 0;


    while (getline(bookingFile, line)) {
        string tempString = "";
        Date date;
        Time time;
        flag=true;

        stringstream inputString(line);

        string RECLOC;

        Date CREATION_DTZ;
        ActionCDs ACTION_CD;
        char CLS_CD;
        int SEG_SEQ, PAX_CNT, FLT_NUM;
        string ORIG_CD;
        string DEST_CD;
        DateTime ARR_DTMZ, DEP_DTMZ;

        getline(inputString, RECLOC, ',');
        int pnr_id=pnrUuidGenerator.getID(RECLOC);

        getline(inputString, tempString, ',');
        CREATION_DTZ = Date(tempString);

        getline(inputString, tempString, ',');
        getline(inputString,tempString, ',');
        ACTION_CD = static_cast<ActionCDs> (getActionCode(tempString));

        getline(inputString,tempString, ',');
        CLS_CD = tempString[0];

        getline(inputString, tempString, ',');
        SEG_SEQ = atoi(tempString.c_str());
        getline(inputString, tempString, ',');
        PAX_CNT = atoi(tempString.c_str());

        if(!pnrMap[pnr_id]){
            Pnr *P = new Pnr(pnr_id,CREATION_DTZ,PAX_CNT);
            pnrMap[pnr_id] = P;
            flag=false;
            prev_seg_seq=0;
        }


        getline(inputString, tempString, ',');

        getline(inputString, tempString, ',');
        FLT_NUM = atoi(tempString.c_str());

        getline(inputString, tempString , ',');
        assert(tempString.size()==CITY_CODE_LENGTH);
        ORIG_CD=tempString;


        getline(inputString, tempString, ',');
        assert(tempString.size()==CITY_CODE_LENGTH);
        DEST_CD=tempString;


        getline(inputString, tempString, ',');
        getline(inputString, tempString, ',');
        getline(inputString, tempString, ',');

        getline(inputString, tempString, ' ');
        date = Date(tempString);
        getline(inputString, tempString, ',');
        time = Time(tempString);
        DEP_DTMZ = DateTime(date, time);

        getline(inputString, tempString, ' ');
        date = Date(tempString);
        getline(inputString, tempString, ',');
        time = Time(tempString);
        ARR_DTMZ = DateTime(date, time);

        line="";

        int inv_id=getFlight(FLT_NUM,DEP_DTMZ, ARR_DTMZ);

        Inventory* I=inventoryMap[inv_id];

        int x=getClassCode(cabinToClassMap[CLS_CD]);

        if(x==0) x=4;

        if(flag && (SEG_SEQ==prev_seg_seq)){
            Journey* J=journeyMap[uuid];
            J->flights.push_back(inv_id);
            J->Dest=DEST_CD;
            J->ClassCD = static_cast <ClassCDs> (min(J->ClassCD,static_cast <ClassCDs> (x)));
        }
        else{
            if(SEG_SEQ<prev_seg_seq) prev_seg_seq=1;
            else prev_seg_seq++;
            Journey* J = new Journey(uuid,ACTION_CD,static_cast <ClassCDs> (x),ORIG_CD,DEST_CD);
            journeyMap[uuid] = J;
            journeyToPnrMap[uuid]=pnr_id;
            J->flights.push_back(inv_id);
            pnrMap[pnr_id]->Journeys.push_back(uuid);
            uuid++;
        }
    }









    //Parsing of Passenger Details File


    ifstream passengerFile;
    passengerFile.open(argv[4]);
    line = "";
    getline(passengerFile, line);
    line.clear();

    uuid=0;

    while (getline(passengerFile, line)){
        stringstream inputString(line);
        string RecLoc;

        string LastName;string FirstName;string Nationality;string PhoneNum;string Email;string DocID; string DocType;
        SpecialNames1 SPECIAL_NAME_CD1;
        SpecialNames2 SPECIAL_NAME_CD2;
        SSRCodes SSR_CODE_CD1;

        string tempString = "";

        getline(inputString, RecLoc, ',');

        int pnr_id = pnrUuidGenerator.getID(RecLoc);

        passengerToPnrMap[uuid] = pnr_id;


        pnrMap[pnr_id]->Passengers.push_back(uuid);

        getline(inputString, tempString, ',');

        getline(inputString, LastName, ',');
        getline(inputString, FirstName, ',');
        getline(inputString, Nationality, ',');
        getline(inputString, PhoneNum, ',');
        getline(inputString, Email, ',');
        getline(inputString, DocID, ',');
        getline(inputString, DocType, ',');

        getline(inputString, tempString, ',');
        SPECIAL_NAME_CD1 = static_cast<SpecialNames1>(getSN1Code(tempString));
        getline(inputString, tempString, ',');
        SPECIAL_NAME_CD2 = static_cast<SpecialNames2>(getSN2Code(tempString));
        getline(inputString, tempString, ',');

        SSR_CODE_CD1 = static_cast<SSRCodes>(getSSRCode(tempString));

        Passenger* P = new Passenger(uuid,LastName,FirstName,Nationality,PhoneNum,Email,DocID,DocType,SPECIAL_NAME_CD1,SPECIAL_NAME_CD2,SSR_CODE_CD1);
        if(true){                                                            //SSR_CODE_CD1 != 1
            Pnr* pnr=pnrMap[pnr_id];
            for(int j_id: pnr->Journeys){
                for(int inv_id: journeyMap[j_id]->flights){
                    Inventory* I=inventoryMap[inv_id];
                    I->BookedInventory++;
                    int x=journeyMap[j_id]->ClassCD;
                    if(x==1) I->FCBookedInventory++;
                    else if(x==2) I->BCBookedInventory++;
                    else if(x==3) I->PCBookedInventory++;
                    else if(x==4) I->ECBookedInventory++;
                }
            }
        }

        passengerMap[uuid] = P;

        uuid++;
    }


    //Graph Creation
    int m6=graphWUGenerator();
    pair<int,int> p1=graphUVAndGraphDVGenerator();
    pair<int,int> p2=graphUCAndGraphCVGenerator();
    int m1=p2.first;
    int m2=p1.first;
    int m3=p2.second;
    int m4=p1.second;
    int m5=graphWDGenerator();

    //Output File Generation
    freopen( argv[5], "r", stdin);
    ofstream fw(argv[6],ofstream::out);

    int no_of_samples;
    cin>>no_of_samples;

    for(int i=0;i<no_of_samples;i++){

        fw<<"Solution"<<i+1<<endl;

        int no_of_uc_edges;
        cin>>no_of_uc_edges;
        for(int j=0;j<no_of_uc_edges;j++){
            int u,c;
            cin>>u>>c;

            int j_id = uIndexGenerator.getID(u);
            fw<<pnrUuidGenerator.getString(journeyToPnrMap[j_id])<<" ";

            for(auto x:cIndexGenerator.getID(c)){
                fw<<inventoryUuidGenerator.getString(x.first)<<" "<<getClass(x.second)<<" ";
            }
            fw<<endl;
        }



        int no_of_uv_edges;
        cin>>no_of_uv_edges;
        for(int j=0;j<no_of_uv_edges;j++){
            int u,v;
            cin>>u>>v;

            int j_id = uIndexGenerator.getID(u);

            pair<int,ClassCDs> pr=vIndexGenerator.getID(v);
            fw<<pnrUuidGenerator.getString(journeyToPnrMap[j_id])<<" "<<inventoryUuidGenerator.getString(pr.first)<<" "<<getClass(pr.second)<<endl;
        }

        int no_of_wd_edges;
        cin >> no_of_wd_edges;
        for (int iter = 0; iter < no_of_wd_edges; ++iter){
            int w, d;
            cin >> w >> d;
            int winvId = wIndexGenerator.getID(w);
            int dinvId = dIndexGenerator.getID(d);
            fw << inventoryUuidGenerator.getString(winvId) << " " << inventoryUuidGenerator.getString(dinvId)<<endl;
        }
    }
    fw.close();

}