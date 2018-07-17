# core
Rover codes 

# Setup instructions

1. After burning the [Raspbian Strech Lite](http://director.downloads.raspberrypi.org/raspbian_lite/images/raspbian_lite-2018-03-14/2018-03-13-raspbian-stretch-lite.zip) on memory card and ```touch SSH``` in boot folder.

2. On any Linux machine access the burned memory card and 
```sudo nano /etc/wpa_supplicant/wpa_supplicant.conf```
add entry at bottom... [more here](https://www.raspberrypi.org/documentation/configuration/wireless/wireless-cli.md)
```
network={
    ssid="wifi_name"
    psk="wifi_password"
}
```

3. After login run increase the memory ```sudo raspi-conig```

4. After this we begin installing deps
```
sudo apt-get update
sudo apt-get upgrade
sudo apt-get install git
git clone https://github.com/teamrudra/core.git
cd core
./install.sh
```
## Setting up GPSD daemon
Change the variables in ```gpsd``` file by ```sudo nano /etc/default/gpsd``` to following:-
```
START_DAEMON="true"
GPSD_OPTIONS="-n"
DEVICES="/dev/ttyUSB0"
# USB Hot plugging (dangerous sometimes)
USBAUTO="false"
GPSD_SOCKET="/var/run/gpsd.sock"
```
Then run the ```sudo systemctl enable gpsd``` to start at every boot or ```sudo systemctl start gpsd``` to start in right away and not at every boot.

## Running GPS2UDP daemon for live updates

```
gps2udp -j -u 192.168.1.29:23907 -b
```

## Setting up static ethernet IP address on pi

Open the dhcpcd.conf by ```sudo nano /etc/dhcpcd.conf``` and add following to end of file :-

```
interface eth0

static ip_address=192.168.1.33/24
static routers=192.168.1.20
static domain_name_servers=192.168.1.20

interface wlan0

static ip_address=192.168.1.23/24
static routers=192.168.1.1
static domain_name_servers=192.168.1.1
```
(optional) for wlan0 entry. [more](https://www.modmypi.com/blog/how-to-give-your-raspberry-pi-a-static-ip-address-update)
```sudo reboot``` to changes take effect.

## Ground Station Settings(Ubiquity)

```
IP Address: 192.168.1.29
Subnet Mask: 255.255.255.0
Gateway: 192.168.1.20
```
> Ubiquity is router, gateway and DNS where IP is 192.168.1.20

> For Windows, Leave DNS settings blank.

## IP Address mapping

| Component       | IP            | Port(Listening)  |
| ----------------|:-------------:| -----:|
| Ground Station  | 192.168.1.29  | 23907 |
| Rover           | 192.168.1.33  | 3301  |
| Ubiquity(Router)| 192.168.1.20  | *     |
