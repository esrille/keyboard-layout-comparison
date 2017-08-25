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
import json


class Layout:
    qwerty = "qwertyuiop[" + "asdfghjkl;'" + "zxcvbnm,./"
    assert len(qwerty) == 32

    def __init__(self, path):
        with open(path) as f:
            self.layout = json.load(f)
        self.normal = self.layout['normal']
        assert len(self.normal) == 32
        if 'shift' in self.layout:
            self.shift = self.layout['shift']
            assert len(self.shift) == 32
        if 'left' in self.layout:
            self.left = self.layout['left']
            assert len(self.left) == 32
        if 'right' in self.layout:
            self.right = self.layout['right']
            assert len(self.right) == 32
        self.daku = self.layout['daku']
        self.handaku = self.layout['handaku']
        self.kogaki = self.layout['kogaki']
        self.prev = ''
        self.cont = True
        if 'cont' in self.layout:
            self.cont = self.layout['cont']
        self.count = 0

    def toqwerty(self, u):
        pos = self.normal.find(u)
        c = ''
        prefix = ''
        if 0 <= pos:
            c = Layout.qwerty[pos]
        elif 'shift' in self.layout:
            pos = self.shift.find(u)
            if 0 <= pos:
                prefix = ' '
                c = Layout.qwerty[pos]
        else:
            pos = self.left.find(u)
            if 0 <= pos:
                prefix = 'L'
                c = Layout.qwerty[pos]
            else:
                pos = self.right.find(u)
                if 0 <= pos:
                    prefix = 'R'
                    c = Layout.qwerty[pos]
        if c:
            self.count += 1
            if prefix and self.prev != prefix or not self.cont:
                print(prefix, sep='', end='')
            print(c, sep='', end='')
            self.prev = prefix

    def convert(self, string):
        for u in string:
            v = ''
            if 0 <= self.daku.find(u):
                u = chr(ord(u) - 1)
                v = '゛'
            elif 0 <= self.handaku.find(u):
                u = chr(ord(u) - 2)
                v = '゜'
            elif 0 <= self.kogaki.find(u):
                u = chr(ord(u) + 1)
                v = '゛'
            self.toqwerty(u)
            if v:
                self.toqwerty(v)


def main():
    if len(sys.argv) < 3:
        print('usage: toqwerty.py layout.json infile')
        return
    layout = Layout(sys.argv[1])
    with open(sys.argv[2]) as file:
        layout.convert(file.read())


if __name__ == '__main__':
    main()
