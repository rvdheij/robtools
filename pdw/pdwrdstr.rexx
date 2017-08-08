/* PDWRDRST REXX                                                     */
/*                   Author: Rob van der Heij, 31 Jul 2017           */

signal on error

'addpipe *.input: | spec 1.4 c2x 1 5.4 c2u nw | *.input:'

do forever
  'peekto rec'
  parse var rec addr len .
  'output' storage(addr,len)
  'readto'
end


error: return rc * ( rc <> 12 )
