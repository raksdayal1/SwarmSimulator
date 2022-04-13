import math, string, os, sys
import whrandom
EPS = 1e-7

def rnd( n):
  return int( whrandom.random() * n)

def run( dt, sdt, te_0, te_dt, te_drift, tmax):
  n_exit = os.system( 'main ' + `dt` + ' ' + `sdt` + ' ' + `tmax` + " " + \
             `te_0` + " "  + `te_dt` + " " + `te_drift` + ' > x.txt')
  if n_exit == 2:
    print "self-detected error in sample.cpp code"
    sys.exit( 1)

  f = open( 'x.txt', 'r')
  data = f.readlines()
  f.close()

  # test events
  t = te_0
  nt = 0
  for line in data:
    if len( line) != 0 and line[:4] == "****":
      #print line
      v = string.split( line)
      nt = nt + 1
      te = float( v[1])
      tp = math.floor( t * 1e6 + 0.5) / 1e6
      if abs( t - tp) > 1e-6 + EPS:
        print "state::EVENT test failed!"
        print "t=%(t)10s te=%(te)10s" % vars()
        sys.exit( 1)
      t = t + ( te_dt + te_drift * ( t - te_0))

  # test periodic sample
  v = string.split( line)
  n = int( v[-1])
  if( int( tmax / sdt + EPS) + 1 == n):
    pass
  else:
    print "sdt test failed!", tmax / sdt, int( tmax / sdt + EPS) + 1, n
    print tmax / sdt - n
    sys.exit( 1)

  # test final answer
  t =  float( v[0])
  ti = float( v[1])
  if abs( t - ti) > t * 1e-9:
    print "final answer failed!"
    print "t =", t, "ti =", ti
    sys.exit( 1)
  
  # if we get to here, everything worked!
  print "test passed", dt, sdt, te_0, te_dt, te_drift, "|", nt, n
  print v


