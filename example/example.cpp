#include <stdio.h>
#include <stdint.h>
#include "jstypes.h"
#include "nanojit.h"

using namespace nanojit;
using namespace std;

const uint32_t CACHE_SIZE_LOG2 = 20;

#if defined(NJ_VERBOSE)
void
nanojit::LirNameMap::formatGuard(LIns *i, char *out)
{
}
#endif

void nanojit::Fragment::onDestroy() {
}

int nanojit::StackFilter::getTop(LInsp guard)
{
  return 0;
}

int main()
{
    // Set up the basic Nanojit objects.
    avmplus::GC *gc = new avmplus::GC;
    if (!gc)
        return 1;
    avmplus::AvmCore core;

#ifdef DEBUG
    core.config.verbose = 1; // Show disassembly of generated traces.
#endif
    Fragmento *fragmento = new (gc) Fragmento(&core, CACHE_SIZE_LOG2);
    LirBuffer *buf = new (gc) LirBuffer(fragmento, NULL);

    // Create a Fragment to hold some native code.
    Fragment *f = new (gc) Fragment(NULL);
    f->lirbuf = buf;
    f->root = f;

    // Create a LIR writer, with verbose output if DEBUG.
    LirBufWriter writer0(buf);

#ifdef DEBUG
    fragmento->labels = new (gc) LabelMap(&core, NULL);
    buf->names = new (gc) LirNameMap(gc, NULL, fragmento->labels);
    VerboseWriter writer(gc, &writer0, buf->names);
#else
    LirBufWriter& writer = writer0;

#endif

    // Write a few LIR instructions to the buffer: add the first parameter
    // to the constant 2.
    writer.ins0(LIR_start);
    LIns *two = writer.insImm(2);
    LIns *firstParam = writer.insParam(0, 0);
    LIns *result = writer.ins2(LIR_add, firstParam, two);
    writer.ins1(LIR_ret, result);

    // Emit a LIR_loop instruction.  It won't be reached, but there's
    // an assertion in Nanojit that trips if a fragment doesn't end with
    // a guard (a bug in Nanojit). 
    LIns *rec_ins = writer0.insSkip(sizeof(GuardRecord) + sizeof(SideExit));
    GuardRecord *guard = (GuardRecord *) rec_ins->payload();
    memset(guard, 0, sizeof(*guard));
    SideExit *exit = (SideExit *)(guard + 1);
    guard->exit = exit;
    guard->exit->target = f;
    f->lastIns = writer.insGuard(LIR_loop, writer.insImm(1), rec_ins);

    // Compile the fragment.
    compile(fragmento->assm(), f);
    if (fragmento->assm()->error() != None) {
        fprintf(stderr, "error compiling fragment\n");
        return 1;
    }
    printf("Compilation successful.\n");

    // Call the compiled function.
    typedef JS_FASTCALL int32_t (*AddTwoFn)(int32_t);
    AddTwoFn fn = reinterpret_cast<AddTwoFn>(f->code());
    printf("2 + 5 = %d\n", fn(5));

    delete buf;
    delete f;
#ifdef DEBUG
    delete fragmento->labels;
#endif
    delete fragmento;
    delete gc;
    return 0;
}
