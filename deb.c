/*    deb.c
 *
 *    Copyright (c) 1991-1999, Larry Wall
 *
 *    You may distribute under the terms of either the GNU General Public
 *    License or the Artistic License, as specified in the README file.
 *
 */

/*
 * "Didst thou think that the eyes of the White Tower were blind?  Nay, I
 * have seen more than thou knowest, Gray Fool."  --Denethor
 */

#include "EXTERN.h"
#define PERL_IN_DEB_C
#include "perl.h"

#if defined(PERL_IMPLICIT_CONTEXT)
void
Perl_deb_nocontext(const char *pat, ...)
{
#ifdef DEBUGGING
    dTHX;
    va_list args;
    va_start(args, pat);
    vdeb(pat, &args);
    va_end(args);
#endif /* DEBUGGING */
}
#endif

void
Perl_deb(pTHX_ const char *pat, ...)
{
#ifdef DEBUGGING
    va_list args;
    va_start(args, pat);
    vdeb(pat, &args);
    va_end(args);
#endif /* DEBUGGING */
}

void
Perl_vdeb(pTHX_ const char *pat, va_list *args)
{
#ifdef DEBUGGING
    dTHR;
    register I32 i;
    GV* gv = PL_curcop->cop_filegv;

#ifdef USE_THREADS
    PerlIO_printf(Perl_debug_log, "0x%lx (%s:%ld)\t",
		  (unsigned long) thr,
		  SvTYPE(gv) == SVt_PVGV ? SvPVX(GvSV(gv)) : "<free>",
		  (long)PL_curcop->cop_line);
#else
    PerlIO_printf(Perl_debug_log, "(%s:%ld)\t",
	SvTYPE(gv) == SVt_PVGV ? SvPVX(GvSV(gv)) : "<free>",
	(long)PL_curcop->cop_line);
#endif /* USE_THREADS */
    (void) PerlIO_vprintf(Perl_debug_log, pat, *args);
#endif /* DEBUGGING */
}

I32
Perl_debstackptrs(pTHX)
{
#ifdef DEBUGGING
    dTHR;
    PerlIO_printf(Perl_debug_log, "%8lx %8lx %8ld %8ld %8ld\n",
	(unsigned long)PL_curstack, (unsigned long)PL_stack_base,
	(long)*PL_markstack_ptr, (long)(PL_stack_sp-PL_stack_base),
	(long)(PL_stack_max-PL_stack_base));
    PerlIO_printf(Perl_debug_log, "%8lx %8lx %8ld %8ld %8ld\n",
	(unsigned long)PL_mainstack, (unsigned long)AvARRAY(PL_curstack),
	(long)PL_mainstack, (long)AvFILLp(PL_curstack), (long)AvMAX(PL_curstack));
#endif /* DEBUGGING */
    return 0;
}

I32
Perl_debstack(pTHX)
{
#ifdef DEBUGGING
    dTHR;
    I32 top = PL_stack_sp - PL_stack_base;
    register I32 i = top - 30;
    I32 *markscan = PL_curstackinfo->si_markbase;

    if (i < 0)
	i = 0;
    
    while (++markscan <= PL_markstack_ptr)
	if (*markscan >= i)
	    break;

#ifdef USE_THREADS
    PerlIO_printf(Perl_debug_log, i ? "0x%lx    =>  ...  " : "0x%lx    =>  ",
		  (unsigned long) thr);
#else
    PerlIO_printf(Perl_debug_log, i ? "    =>  ...  " : "    =>  ");
#endif /* USE_THREADS */
    if (PL_stack_base[0] != &PL_sv_undef || PL_stack_sp < PL_stack_base)
	PerlIO_printf(Perl_debug_log, " [STACK UNDERFLOW!!!]\n");
    do {
	++i;
	if (markscan <= PL_markstack_ptr && *markscan < i) {
	    do {
		++markscan;
		PerlIO_putc(Perl_debug_log, '*');
	    }
	    while (markscan <= PL_markstack_ptr && *markscan < i);
	    PerlIO_printf(Perl_debug_log, "  ");
	}
	if (i > top)
	    break;
	PerlIO_printf(Perl_debug_log, "%-4s  ", SvPEEK(PL_stack_base[i]));
    }
    while (1);
    PerlIO_printf(Perl_debug_log, "\n");
#endif /* DEBUGGING */
    return 0;
}
