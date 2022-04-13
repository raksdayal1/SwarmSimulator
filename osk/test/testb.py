#!/usr/bin/env python
import string, os, sys
import whrandom
import test

def run_test( TEST):
  whrandom.seed( 198)
  N = 100
  tmax = 10.0
  ff = 1
  dts = [ 0.1, 0.01, 0.001, 0.0001, 0.00001, 0.000001,
          0.2, 0.02, 0.002, 0.0002, 0.00002, 0.000002,
          0.3, 0.03, 0.003, 0.0003, 0.00003, 0.000003,
          1.0 / 300, 1.0 / 400, 1.0 / 600, 1.0 / 450,
          1.0 / 700, 1.0 / 800, 1.0 / 900]
  sdts = [ 0.1, 0.01, 0.001, 0.0001, 0.00001, 0.000001,
           0.333333, 0.033333, 0.003333]
  te_0s = [ 0.0, 0.1, 0.01, 0.001, 1.001, 1.2, 1.3, 2.001, 3.4, 19.0, 99.0]
  te_dts = [ 0.1, 0.01, 0.234, 0.234e-2, 0.333333, 0.001]
  te_drifts = [ 0.0, 0.0, 0.0, 0.1, 0.01, 0.02, 0.03, 0.2, 0.003333]

  if TEST == 0:
    tmax = 10.0
    ff = 1

  if TEST == 1:
    tmax = 100.0
    ff = 10

  if TEST == 2:
    tmax = 1000.0
    ff = 100

  if TEST == 3:
    tmax = 10000.0
    ff = 1000

  if TEST == 4:
    whrandom.seed( 234)
    tmax = 100000.0
    ff = 10000

  if TEST == 5:
    whrandom.seed( 238)
    tmax = 1000000.0
    ff = 100000

  if TEST == 6:
    N = 20
    whrandom.seed( 138)
    tmax = 1000000.0
    ff = 10000

  if TEST == 7:
    dts = [ 0.1, 0.01, 0.001, 0.0001, 0.00001, 0.000001,
            0.2, 0.02, 0.002, 0.0002, 0.00002, 0.000002,
            0.3, 0.03, 0.003, 0.0003, 0.00003, 0.000003]
    sdts = [ 1 / 300.0, 1 / 400.0, 1 / 600.0, 1 / 700.0, 1 / 450.0,
             1 / 800.0, 1 / 900.0]
    te_0s = [ 0.0, 0.1, 0.01, 0.001, 1.001, 1.2, 1.3, 2.001, 3.4, 19.0, 99.0]
    te_dts = [ 1 / 300.0, 1 / 400.0, 1 / 600.0, 1 / 700.0, 1 / 450.0,
             1 / 800.0, 1 / 900.0]
    te_drifts = [ 0.0, 0.0, 0.0, 0.1, 0.01, 0.02, 0.03, 0.2, 0.003333]
    N = 100
    whrandom.seed( 128)
    tmax = 10.0
    ff = 1

  cases = []
  ii = 0
  nn = 0
  while 1:
    dt = dts[ test.rnd( len( dts))] * ff
    sdt = sdts[ test.rnd( len( sdts))] * ff
    te_0 = te_0s[ test.rnd( len( te_0s))] * ff
    te_dt = te_dts[ test.rnd( len( te_dts))] * ff
    te_drift = te_drifts[ test.rnd( len( te_drifts))] * ff

    val = [ dt, sdt, te_0, te_dt, te_drift]
    if not( val in cases):
      cases.append( val)
      print 'run #', ii,
      test.run( dt, sdt, te_0, te_dt, te_drift, tmax)
      ii = ii + 1
    else:
      print "repeat:", val
      nn = nn + 1
    if ii == N:
      break

  print "total number of repeats =", nn

if __name__ == "__main__":
  tests = [ 0, 1, 2, 3, 4, 5, 6, 7]
  for n in tests:
    print "#############################################"
    print "Running test", n
    print "#############################################"
    run_test( n)



