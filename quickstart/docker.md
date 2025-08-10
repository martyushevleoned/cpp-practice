# Docker

## install
```shell
sudo groupadd docker
sudo usermod -aG docker $USER
wget --output-document=/tmp/get-docker.sh https://get.docker.com
sudo sh /tmp/get-docker.sh
```

## list
```shell
docker images
docker container list
docker network list
```