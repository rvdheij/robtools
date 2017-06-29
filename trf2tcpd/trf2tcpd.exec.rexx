/* TRF2TCPD EXEC          Read TCP/IP Packet Trace files and convert */
/*                        Author: RVDHEIJ, 9 Jun 2005                */

arg spid fn ft fm .                  /* TRF files separated by comma */
if ft = '' then signal usage
parse value fm 'A' with fm .


'PIPE query level | var lvl'

reqlvl = '110B000B'x                /* This has the trfread stage in */
if lvl << reqlvl then
  do
    say 'CMS Pipelines must be at' c2x(reqlvl) 'to ride this attraction'
    exit 1
  end

'PIPE (end \ name TRF2TCPD.EXEC:18)',
   '\ var spid ',
   '| split , ',
   '| spec ,\\ trfread, 1 w1 nw ,|| i:, nw ',
   '| join * | var inputs'

'PIPE (end \ name TRF2TCPD.EXEC:24)',
   '\ i: fanin ',
   '| trf2tcpd ',
   '| >' fn ft fm,
   inputs

return rc

usage:
  say 'Convert TCP/IP Packet Trace files into tcpdump format'
  say '   TRF2TCPD spids fn ft fm'
  say ' '
  say '   spids       TRF spool files (separate with commas)'
  say '   fn ft fm    Output CMS file'
