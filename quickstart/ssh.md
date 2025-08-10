# ssh

## key
```shell
ssh-keygen
ssh-copy-id <user>@<host>
```

## Config
* подключение по ключу
```config
Host <name>
    HostName <host>
    User <username>
    IdentityFile ~/.ssh/<key>
```
* socks5 прокси через ssh туннель
```config
Host proxy
    HostName <host>
    User <username>
    IdentityFile ~/.ssh/<key>
    DynamicForward <port>
```
* запуск туннеля
* `-f` - фоновый режим
* `-N` - не выполнять команды
```shell
ssh -fN proxy
```