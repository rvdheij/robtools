/* TRF2TCPD REXX          Convert TCP/IP Packet trace to tcpdump     */
/*                        Author: RVDHEIJ, 4 Apr 2005                */
maxlen = 256
linktp = 101

numeric digits 12
day0 = date('B','01/01/70','U')       /* UNIX start of time          */

output 'a1b2c3d4'x ,                  /* tcpdump magic: big endian   */
   || '0002'x || '0004'x,             /* pcap version 2.4            */
   || '00000000'x ,                   /* offset from GMT in seconds  */
   || '00000000'x ,                   /* time significance ?         */
   || d2c(maxlen, 4),                 /* Maximum snapped length      */
   || d2c(linktp, 4)                  /* data link type              */

'callpipe (end \ name TRF2TCPD.REXX:5)',
   '\ *:',                            /* Output of trfread           */
   '| not chop 4 ',                   /* Dunno what this one is      */
   '| hdr: not chop 32',              /* Remove header               */
   '| deblock rdw 1.2 strip ',        /* Use the tracered length     */
   '| len: not chop 2',               /* Take off packet length      */
   '| tod: chop 8',                   /* Take the TOD clock          */
   '| spec 1.8 c2t ',                 /* and make it readable        */
   '| t: chop 11',                    /* The dates are sorted so we  */
   '| unique first',                  /* only compute new dates      */
   '| dateconvert w1 iso rexxb',      /* Date into Basedate          */
   '| spec a: w1 -',
        'print a-'day0' 1',           /* Days since 01/01/1970       */
   '| j: juxtapose',                  /* with time portion           */
   '| copy',
   '| spec a: w1 - b: w2 - c: w3 -',
        'print a*86400+b d2c 1.4',    /* Seconds since 01/01/1970    */
        'print c d2c n.4',            /* Number of microseconds      */
   '| s: spec 1-* 1 pad 00',          /* The timeval struct          */
        'select 1 x0000 13 1.2 15.2', /* and packet length           */
        'select 2 1.4 9',             /* captured length             */
                 '5-* 17',            /* and packet data             */
   '| *:',
   '\ len:',                          /* Original packet length      */
   '| copy',
   '| s:',
   '\ tod:',                          /* The actual packet           */
   '| addrdw cms4 ',                  /* Prefix with length          */
   '| s:',
   '\ t: ',                           /* Time portion of timestamp   */
   '| spec h: 1.2 - m: 4.2 - s: 7.2 -',
          'print (h*60+m)*60+s 2 10.6 nw',
   '| j:',


