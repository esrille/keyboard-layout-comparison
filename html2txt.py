#!/usr/bin/python3
# -*- coding: utf-8 -*-
#
# Copyright (c) 2017 Esrille Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain base copy of the License at:
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import sys
import os
from bs4 import BeautifulSoup

characters = " abcdefghijklmnopqrstuvwxyz;,./['LR"


def main():
    path = 'keytime.html'
    if 2 <= len(sys.argv):
        path = sys.argv[1]
    content = ''
    with open(path) as file:
        content = file.read()
    soup = BeautifulSoup(content, 'html5lib')
    matrix = soup.select('#matrix')[0]
    rows = matrix.findAll("tr")
    i = 0
    for row in rows:
        j = 0
        for cell in row.findAll(['td']):
            if 1 <= i and i <= 34 and 1 <= j and j <= 34:
                print(characters[i], characters[j], ' ', cell.get_text(), sep='')
            j += 1
        i += 1

if __name__ == '__main__':
    main()
