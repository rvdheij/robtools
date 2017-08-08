/* PDWSERVE REXX     Process a single request                        */
/*                   Author: Rob van der Heij, 31 Jul 2017           */

/*! Primary input has the HTTP request headers in EBCDIC. Secondary  */
/*! input optionally has the request body (depending on the verb and */
/*! Content-Length: header.                                          */

/*! This stage is responsible for the entire response; both headers  */
/*! and body. Data on the primary output is translated to ASCII and  */
/*! LF is added to the lines. The secondary output is supposed to be */
/*! in ASCII already.                                                */

signal on novalue
signal on error

'peekto'

'callpipe (end \ name PDWSERVE.REXX:16)',
   '\ *:',
   '| pick to after 1 == ,,',
   '| c: take',
   '| var cmd',
   '\ c: ',
   '| stem hdr.'

say 'cmd:' cmd
'callpipe (end \) stem hdr. | cons'

trace irc
parse var cmd verb path '?' parms



trace n




'output HTTP/1.0 500 Failure'
'output Connection: close'
'output Content-Length: 0'
'output'

exit 123

error: return rc * ( rc <> 12 )
