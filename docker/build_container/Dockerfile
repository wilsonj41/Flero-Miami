# syntax=docker/dockerfile:1.3-labs
FROM ubuntu/apache2:latest
RUN apt-get update && apt-get install -y build-essential libapache2-mod-fcgid apache2-dev libfcgi-dev && a2enmod ssl && a2enmod proxy && a2enmod proxy_balancer && a2enmod lbmethod_byrequests

