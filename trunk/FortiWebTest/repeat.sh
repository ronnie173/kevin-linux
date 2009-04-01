#!/bin/sh
# usage: repeat command sleep_period
while true; do $1; sleep $2; done