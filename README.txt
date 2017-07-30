rlrl??
ready
WIFI模块初始化超时

AT+CWJAP="CandyTime_7ED191","ktlovejackiemilktea&coffee"
AT+CWJAP="KLX","klx39909679203"

OK
AT+CIPSTART="TCP","192.168.100.15",80
AT+CIPSTART="TCP","192.168.199.145",80
CONNECT

OK
AP连接超时

AT+CIPMODE=1

OK
AT+CIPSEND

OK

POST http://localhost/LoginTest/connection-test.php HTTP/1.0


>HTTP/1.1 200 OK
Date: Sat, 29 Jul 2017 03:02:44 GMT
Server: Apache/2.4.23 (Win64) PHP/5.6.25
X-Powered-By: PHP/5.6.25
ContentCLOSED
-Length: 26
Connection: close
Content-Type: text/html; charset=UTF-8

hello device!!
Array
(
)

AT+CWLAP