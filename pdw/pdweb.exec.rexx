/* PDWEB    EXEC     Pipedemo Web Service                            */
/*                   Author: Rob van der Heij, 31 Jul 2017           */


signal on error
signal on novalue
address command

vers = '1.0.0'
call minlevel '110B000E'x             /* For httpsplit 1.1.11/0E     */

/*! Get the workstation IP address from Q V CONS and have ip2socka   */
/*! validate it and build me the 4-byte address for the check.       */

'PIPE (end \ name PDWEB.EXEC:15)',
   '\ cp QUERY VIRT CONS',            /* Client IP address           */
   '| pick 52.4 == ,FROM,',           /* Ensure IP address present   */
   '| take ',
   '| spec ,AF_INET 0, 1 w11 nw ',
   '| locate w1',
   '| ip2socka ',
   '| spec ,x, 1 5.4 c2x n',
   '| var ipx',
   '| count lines',
   '| var ok'

if \ok then call msg 32, 'Unable to see workstation IP address'

parse value value('PORT',,'GLOBAL PDWEB') '0',
      with port .                     /* Known port?                 */
do
  rc = process(port)
  if rc <> '1015' | port <> 0 then leave
  port = 0                            /* Try again with random port  */
end



error: exit rc

process:
  parse arg port .
  signal off error

  path = ''

  'PIPE (end \ name PDWEB.EXEC:44)',
     '\ tcplisten' port 'getsockname',/* Listen for connection       */
     '| o: take',
     '| spec 3.2 c2u 1',              /* Socket TCPIP selected       */
     '| var newport',
     '| preface hostid',
     '| append var path',
     '| spec ,See http://, 1 w1 n read ,:, n w1 n ,/, n read 1-* n',
            ',or PIPMOD STOP to stop it., nw',
     '| cons',
     '\ o: ',
     '| pick 69.4 ==' ipx ,           /* Wait for my connection      */
     '| pdwspawn',
     '| cons'

  if rc = 0 & port = 0 then t = value('PORT',newport,'LASTING PDWEB')

return rc

msg: procedure
  parse arg rc , txt
  say txt
  if rc = 0 then return
  exit rc

minlevel: procedure
  parse arg req .
  'PIPE query level | var qlevel'
  if qlevel >= req then return
  parse value c2x(req) with v +1 r +1 m +2 sub +4
  txt = x2d(v)'.'x2d(r)'.'x2d(m)'/'sub
  call msg 32, 'Requires CMS Pipelines at level' txt 'or later'



