#!/usr/bin/python
# -*- coding: utf-8 -*-
# --------------------------------------
"""EmbLib32 repository management tasks

:date:      2024
:author:    Christian Wiche
:contact:   cwichel@gmail.com
:license:   The MIT License (MIT)
"""
# --------------------------------------
__author__ = "Christian Wiche"
__version__ = "ALPHA"
# --------------------------------------

# Built-in
import pathlib as pl

# 3rd party
import invoke as inv


# -->> Tunables <<----------------------


# -->> Definitions <<-------------------
PATH_REPOSITORY: pl.Path = pl.Path(__file__).parent
"""Repository ROOT path."""


# -->> Backend <<-----------------------


# -->> Tasks <<-------------------------


# -->> Invoke Namespace <<--------------
idt = [
]
"""tp.List[tp.Callable]: Invoke Default Tasks. These tasks need to be always available"""

ns = inv.Collection(*idt)
"""inv.Collection: Invoke NameSpace. These tasks can be listed by the user using 'invoke --list'"""
