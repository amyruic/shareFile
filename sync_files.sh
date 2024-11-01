#!/bin/bash

git add .
git commit -m "Sync changes"
git push

ssh -p 8076 ldayr2@bits-head.cs.uic.edu "cd shareFile/ && git pull"
