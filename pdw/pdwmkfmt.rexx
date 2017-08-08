/* MDXMKFMT REXX      Build specification items for variables        */
/*                    Author: Rob van der Heij, 3 May 2017           */

/* Input stream consists of the member name and type indicator. Each */
/* output record is the specification item for that member, with the */
/* proper formatting and routing.                                    */
/* The specification items are to be used in a SPEC stage that has   */
/* different output streams for numeric values and string values.    */


/* The generated specification items expect a SPEC stage that has    */
/* three output streams defined:                                     */
/* .0:   the name of the field being shown                           */
/* num:  numeric values that can be shown as-is                      */
/* str:  string values to be stripped, escaped and quoted            */

'callpipe (end \ name MDKMKFMT.REXX:4)',
   '\ *:',
   '| o: fanout ',
   '| l: lookup w2 w1 master',        /* Find formatting code        */
   '| y: faninany',  /* y num,print,sqrt(,)/1e3 pic -----------9.999 */
   '| copy',
   '| s: spec ,outstream, 1 fs , substr f1 of w2 nw',     /* num/str */
            'substr f2 of w2 nw substr f3 of w2 nw',
            'select 1 1-* nw',        /* Field name                  */
            'select 0 fs , substr f4 of w2-* n ,1 write, nw',
   '| i: faninany',
   '| *:',
   '\ o: ',
   '| substr w1 ',
   '| copy',
   '| s:',
   '\ < pdwmkfmt txt',                /* Formatting table            */
   '| l: ',
   '| spec .X   str,mem,, c2x.',
   '| y:',

error: return rc * ( rc <> 12 )
