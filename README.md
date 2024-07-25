 Self Watering Plant System

This project uses a Raspberry Pi, along with a Ecowitt soil sensor to control a pump to water plants. 


## Acknowledgements

 - [Wiring Tutorial](https://docs.viam.com/tutorials/projects/make-a-plant-watering-robot/)
 - [Ecowitt Sensor](https://www.amazon.com/ECOWITT-Moisture-Sensor-Humidity-Tester/dp/B07JM621R3?source=ps-sl-shoppingads-lpcontext&ref_=fplfs&psc=1&smid=A13RMNHJJ617OY)


## Package Dependancies
 * Basic C++ Library
 * Libcurl
 * pigpio (Debian Only)

## Hardware Setup
Using the wiring tutorial above, I connected the Raspberry Pi to a relay, and the relay to a 5v pump. 

Using the Ecowitt Soil sensor and WB1100, go through the setup on the device, and connect the network to your own local network. 

Connect Raspberry Pi to the network created by the WB1100.

It is important to calibrate the sensor, and find a "dry" and "moist" reading. Then change the values in the updateData function and waterPlants to change the dry level and how long the pump should run.

Note: The GPIO pin in the code should be changed to the pin the relay "IN" is connected to.

## Run Locally

Clone the project to Raspberry Pi

```bash
  git clone https://github.com/dgrassy/Water
```

Go to the project directory

```bash
  cd Water
```

Install dependencies

```bash
  npm install
```

Install Tmux

```bash
  apt install tmux
```

SSH into Raspberry Pi from another device

Start tmux session

```bash
  tmux
```

Start the program

```bash
  sudo ./Water
```

Disconnect from tmux session
