#pragma once
#include "data.h"
#include "timeDiff.h"
using namespace std;

int pnrScore(int journeyId, CLASS_CD proposed){
    Journey* journey = journeyMap[journeyId];
    Pnr* pnr = pnrMap[journeyToPnrMap[journeyId]];
    int pnr_score = 0;
    for (auto &passenger_id: pnr->passengers){
        Passenger* passenger = passengerMap[passenger_id];
        pnr_score += (passenger->ssrCodes.size()*SSR_SCORE);
        pnr_score += PAX_SCORE;
    }
    pnr_score += classScoresMap[proposed];
    return pnr_score;
}

long long getFlightScoreWithTimeDiff(Time ArrivalTimeDiff, Time DepartureTimeDiff){
    long long score = 0;

    if (ArrivalTimeDiff <= Time(6, 0))       score += ARRIVAL_DELAY_LT_6_SCORE;
    else if (ArrivalTimeDiff <= Time(12, 0)) score += ARRIVAL_DELAY_LT_12_SCORE;
    else if (ArrivalTimeDiff <= Time(24, 0)) score += ARRIVAL_DELAY_LT_24_SCORE;
    else if (ArrivalTimeDiff <= Time(48, 0)) score += ARRIVAL_DELAY_LT_48_SCORE;

    if (DepartureTimeDiff <= Time(6, 0))       score += DEPARTURE_DELAY_LT_6_SCORE;
    else if (DepartureTimeDiff <= Time(12, 0)) score += DEPARTURE_DELAY_LT_12_SCORE;
    else if (DepartureTimeDiff <= Time(24, 0)) score += DEPARTURE_DELAY_LT_24_SCORE;
    else if (DepartureTimeDiff <= Time(48, 0)) score += DEPARTURE_DELAY_LT_48_SCORE;

    return score;
}

long long getFlightScore(int originalinvId, int proposedinvId){
    long long score = 0;

    Time ArrivalTimeDiff = getArrTimeDiff(originalinvId, proposedinvId);
    Time DepartureTimeDiff = getDepTimeDiff(originalinvId, proposedinvId);
    
    score += getFlightScoreWithTimeDiff(ArrivalTimeDiff, DepartureTimeDiff);

    score += CITYPAIR_SCORE;
    score += scheduleMap[inventoryToScheduleMap[originalinvId]]->equipmentNo ==
             scheduleMap[inventoryToScheduleMap[proposedinvId]]->equipmentNo ? EQUIPMENT_SCORE: 0;

    return score;
}

long long getConnectingFlightScore(int originalInventoryID, vector<pair<int, CLASS_CD>> connectingFlight){
    long long score = 0;

    Time ArrivalTimeDiff = getArrTimeDiff(connectingFlight[0].first, originalInventoryID);
    Time DepartureTimeDiff = getDepTimeDiff(connectingFlight.back().first, originalInventoryID);
    
    score += getFlightScoreWithTimeDiff(ArrivalTimeDiff, DepartureTimeDiff);

    score += CITYPAIR_SCORE;

    return score;
}

long long getFinalConnectingFlightScore(int journeyID, vector<pair<int, CLASS_CD>> proposedFlight){
    long long originalPnrScore = pnrScore(journeyID, journeyMap[journeyID]->classCD);

    long double avgPnrScore = 0;
    long double totalTime = 0;

    CLASS_CD originalClassCD = journeyMap[journeyID]->classCD;

    for(auto [curFlightID, curClassCD]: proposedFlight){
        long double curTime = getFlightDuration(curFlightID).value();
        totalTime += curTime;
        avgPnrScore += (curTime*pnrScore(journeyID, curClassCD));
    }

    avgPnrScore /= totalTime;

    return originalPnrScore*getConnectingFlightScore(journeyID, proposedFlight)*avgPnrScore;
}