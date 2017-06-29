/* TRC2TCPD REXX     Convert TRACERED output to pcap format          */
/*                   Author: Rob van der Heij, 29 Jun 2017           */
                                                                       
signal on error                                                        
signal on novalue                                                      
                                                                       
/* While it is much easier to pipe the TRF file straight into a pipe */
/* some people use TRACERED first to make a CMS file and XEDIT it.   */
/* If you did, then this transforms the CMS file back into raw TRF   */
/* format so trf2tcpd can process it.                                */
                                                                       
'peekto'                                                               
'callpipe (end \ name TRC2TCPD.REXX:8)',                               
   '\ *:',                                                             
   '| nlocate /-----/ ',              /* Remove decorations          */
   '| strip trailing ',               /* .. and other frivalty       */
   '| locate w1 ',                    /* .. like blank lines         */
   '| spec 71.4 1 26-69 5',           /* SPID tag front              */
   '| joincont not leading /SPID/',   /* Reconstruct fragments       */
   '| substr 5-*',                    /* Remove SPID tag             */
   '| space 0',                                                        
   '| spec 1-* x2c ',                 /* Back to binary              */
   '| block 32731 sf',                /* 32767 minus 36 byte header  */
   '| spec pad 00 1-* 37',            /* Make header                 */
   '| *:'                                                              
                                                                       
error: return rc * ( rc <> 12 )                                        
