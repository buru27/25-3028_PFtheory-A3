#include <stdio.h>

int calculateFuel(int fuel, int consumption, int recharge, int solarBonus, int planet, int totalPlanets) {
    if (fuel <= 0) {
        printf("Planet %d: Fuel Remaining = 0 (exhausted)\n", planet);
        return 0;
    }
    if (planet > totalPlanets) {
        printf("All %d planets visited. Fuel Remaining = %d\n", totalPlanets, fuel);
        return 1;
    }
    fuel -= consumption;
    if (fuel < 0) fuel = 0;
    if (planet % 5 == 0) {
        fuel += recharge;
    }
    if (planet % 4 == 0) {
        fuel += solarBonus;
    }
    printf("Planet %d: Fuel Remaining = %d\n", planet, fuel);
    if (fuel <= 0) {
        printf("Fuel exhausted at planet %d\n", planet);
        return 0;
    }
    return calculateFuel(fuel, consumption, recharge, solarBonus, planet + 1, totalPlanets);
}

int main() {
    int startFuel = 1000;
    int consumption = 200;
    int recharge = 150;
    int solarBonus = 100;
    int totalPlanets = 7;
    int ok = calculateFuel(startFuel, consumption, recharge, solarBonus, 1, totalPlanets);
    if (ok) printf("Journey completed successfully.\n");
    else printf("Journey failed due to fuel exhaustion.\n");
    return 0;
}
