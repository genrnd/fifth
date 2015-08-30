#!/usr/bin/python

import sys
import os
import struct

##
#
#
def verify_command( command ):
  cmd_args_cnt_dict = { "push"    : 1, 
                        "drop"    : 0,
                        "dup"     : 0,
                        "swap"    : 0,
                        "over"    : 0,
                        "rot"     : 0,
                        "add"     : 0,
                        "mul"     : 0,
                        "sub"     : 0,
                        "inc"     : 0,
                        "dec"     : 0,
                        "lt"      : 0,
                        "gt"      : 0,
                        "eq"      : 0,
                        "ne"      : 0,
                        "cjump"   : 0,
                        "goto"    : 0,
                        "nop"     : 0,
                        "print"   : 0,
                        "lrect"   : 0,
                        "mrect"   : 0,
                        "brect"   : 0,
                        "fill"    : 0,
                        "char"    : 0,
                        "delay"   : 0,
                        "led"     : 0,
                        "sound"   : 0,
                        "rand"    : 0,
                        "peek"    : 0,
                        "poke"    : 0,
                        "tpeek"   : 0,
                        "tpoke"   : 0}

  try:
    arg_cnt = cmd_args_cnt_dict[ command ]
  except:
    arg_cnt = -1

  return arg_cnt

##
#
#
def verify_arg( arg ):
  try:
    int( arg, 0 )
    return 0
  except:
    return -1

##
#
#  note: line is already stripped
#
def is_comment( line ):
  if line[ 0 ] == '#':
    return 1

  return 0


##
#
#
def parse_line( line, line_idx ):

  # check if line is empty
  if not line.strip( ):
    return 1

  line = line.strip( ' ' )

  if is_comment( line ) == 1:
    return 1;

  line_list = line.split( )
  command = line_list[ 0 ]
  args = line_list[ 1: ]

  arg_cnt = verify_command( command )

  if arg_cnt == -1:
    print "Wrong command '%s' in line %d" % ( command, line_idx )
    return -1

  if len( args ) != arg_cnt:
    print "Wrong argument count in line %d: given %d, expected %d" % \
        ( line_idx, len( args ), arg_cnt )
    return -1

  args_ok = 1
  for arg in args:
    if verify_arg( arg ) != 0:
      print "Wrong argument '%s' in line %d: not an integer" % \
            ( arg, line_idx )
      args_ok = 0

  if args_ok == 0:
    return -1

  return 0

## 
#
#
def convert_cmd( line ):
  line_list = line.split( )
  command = line_list[ 0 ]
  args = line_list[ 1: ]

  cmd_codes_dict = { "push"    : 0x0, 
                     "drop"    : 0x1,
                     "dup"     : 0x2,
                     "swap"    : 0x3,
                     "over"    : 0x4,
                     "rot"     : 0x5,
                     "add"     : 0x10,
                     "mul"     : 0x11,
                     "sub"     : 0x12,
                     "inc"     : 0x13,
                     "dec"     : 0x14,
                     "lt"      : 0x20,
                     "gt"      : 0x21,
                     "eq"      : 0x22,
                     "ne"      : 0x23,
                     "cjump"   : 0x30,
                     "goto"    : 0x31,
                     "lrect"   : 0x40,
                     "mrect"   : 0x41,
                     "brect"   : 0x42,
                     "fill"    : 0x43,
                     "char"    : 0x44,
                     "delay"   : 0x45,
                     "led"     : 0x46,
                     "sound"   : 0x47, 
                     "nop"     : 0x50,
                     "print"   : 0x51,
                     "rand"    : 0x52,
                     "peek"    : 0x60,
                     "poke"    : 0x61,
                     "tpeek"   : 0x62,
                     "tpoke"   : 0x63,
                     }

  try:
    cmd_code = cmd_codes_dict[ command ]
  except:
    print "Can't find code for valid command %s" % command
    sys.exit( 1 )

  cmd_packer = struct.Struct( 'B' )
  cmd_bin = cmd_packer.pack( cmd_code )
  os.write( sys.stdout.fileno(), cmd_bin )

  arg_packer = struct.Struct( 'B' )
  for arg in args:
    arg_bin = arg_packer.pack( int( arg, 0 ) )
    os.write( sys.stdout.fileno(), arg_bin )
      
##
#
#
if __name__ == "__main__":

  input_filename = sys.argv[1]
  input_file = open( input_filename, "r" )

  line_idx = 1
  cmd_list = []

  for line in input_file:
    # TODO: we should skip comments but abort in case of errors 
    res = parse_line( line, line_idx )
    if res < 0:
      print "Program conversion error. Abort"
      sys.exit( 1 )

    if res == 0:
      cmd_list.append( line.strip( ' ' ) )

    line_idx += 1

  for cmd in cmd_list:
    convert_cmd( cmd )

  end_packer = struct.Struct( 'B' )
  end_bin = end_packer.pack( 0xa )
  os.write( sys.stdout.fileno(), end_bin )

