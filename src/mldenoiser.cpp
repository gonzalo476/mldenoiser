

#include "mldenoiser.h"
#include "Utils.h"

#include <iostream>
#include <iomanip>

MLDenoiserIop::MLDenoiserIop(Node *node) : PlanarIop(node) {};

void MLDenoiserIop::knobs(Knob_Callback f)
{
    Text_knob(f, "mldenoiser", "");
}

void MLDenoiserIop::_validate(bool for_real)
{
    if (input(0))
    {
        input(0)->validate(for_real);
    }

    info_.channels(Mask_RGB);
    copy_info();
}

void MLDenoiserIop::getRequests(const Box &box, const ChannelSet &channels, int count, RequestOutput &reqData) const
{
    reqData.request(input(0), box, channels, count);
};

void MLDenoiserIop::renderStripe(ImagePlane &imagePlane)
{
    input0().fetchPlane(imagePlane);

    imagePlane.makeWritable();

    Box box = imagePlane.bounds();

    foreach (z, imagePlane.channels())
    {
        for (Box::iterator it = box.begin(); it != box.end(); it++)
        {
            imagePlane.writableAt(it.x, it.y, imagePlane.chanNo(z)) = 1.0f;
        }
    }
}

static Iop *build(Node *node) { return new MLDenoiserIop(node); }
const Iop::Description MLDenoiserIop::d("MLDenoiser", "MLDenoiser", build);
