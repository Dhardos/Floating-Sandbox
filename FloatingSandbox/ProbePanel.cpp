/***************************************************************************************
* Original Author:      Gabriele Giuseppini
* Created:              2018-09-07
* Copyright:            Gabriele Giuseppini  (https://github.com/GabrieleGiuseppini)
***************************************************************************************/
#include "ProbePanel.h"

#include <wx/stattext.h>

#include <cassert>

static constexpr int TopPadding = 2;
static constexpr int ProbePadding = 10;

ProbePanel::ProbePanel(wxWindow* parent)
    : wxPanel(
        parent,
        wxID_ANY,
        wxDefaultPosition,
        wxDefaultSize,
        wxBORDER_SIMPLE | wxCLIP_CHILDREN)
{
    SetDoubleBuffered(true);

    SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));


    //
    // Create probes
    //

    mProbesSizer = new wxBoxSizer(wxHORIZONTAL);

    mFrameRateProbe = AddScalarTimeSeriesProbe("Frame Rate", 200);
    mURRatioProbe = AddScalarTimeSeriesProbe("U/R Ratio", 200);

    mWaterTakenProbe = AddScalarTimeSeriesProbe("Water Inflow", 120);
    mWaterSplashProbe = AddScalarTimeSeriesProbe("Water Splash", 200);

    mWindSpeedProbe = AddScalarTimeSeriesProbe("Wind Speed", 200);

    //
    // Finalize
    //

    SetSizerAndFit(mProbesSizer);
}

ProbePanel::~ProbePanel()
{
}

void ProbePanel::Update()
{
    //
    // Update all probes
    //

    if (IsActive())
    {
        mFrameRateProbe->Update();
        mURRatioProbe->Update();
        mWaterTakenProbe->Update();
        mWaterSplashProbe->Update();
        mWindSpeedProbe->Update();

        for (auto const & p : mCustomProbes)
        {
            p.second->Update();
        }
    }
}

std::unique_ptr<ScalarTimeSeriesProbeControl> ProbePanel::AddScalarTimeSeriesProbe(
    std::string const & name,
    int sampleCount)
{
    wxBoxSizer * sizer = new wxBoxSizer(wxVERTICAL);

    sizer->AddSpacer(TopPadding);

    auto probe = std::make_unique<ScalarTimeSeriesProbeControl>(this, sampleCount);
    sizer->Add(probe.get(), 1, wxALIGN_CENTRE, 0);

    wxStaticText * label = new wxStaticText(this, wxID_ANY, name, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
    sizer->Add(label, 0, wxALIGN_CENTRE, 0);

    mProbesSizer->Add(sizer, 1, wxLEFT | wxRIGHT, ProbePadding);

    return probe;
}

///////////////////////////////////////////////////////////////////////////////////////

void ProbePanel::OnGameReset()
{
    mFrameRateProbe->Reset();
    mURRatioProbe->Reset();
    mWaterTakenProbe->Reset();
    mWaterSplashProbe->Reset();
    mWindSpeedProbe->Reset();

    for (auto const & p : mCustomProbes)
    {
        p.second->Reset();
    }
}

void ProbePanel::OnWaterTaken(float waterTaken)
{
    mWaterTakenProbe->RegisterSample(waterTaken);
}

void ProbePanel::OnWaterSplashed(float waterSplashed)
{
    mWaterSplashProbe->RegisterSample(waterSplashed);
}

void ProbePanel::OnWindSpeedUpdated(
    float const /*zeroSpeedMagnitude*/,
    float const /*baseSpeedMagnitude*/,
    float const /*preMaxSpeedMagnitude*/,
    float const /*maxSpeedMagnitude*/,
    vec2f const & windSpeed)
{
    mWindSpeedProbe->RegisterSample(windSpeed.length());
}

void ProbePanel::OnCustomProbe(
    std::string const & name,
    float value)
{
    auto & probe = mCustomProbes[name];
    if (!probe)
    {
        probe = AddScalarTimeSeriesProbe(name, 100);
        mProbesSizer->Layout();
    }

    probe->RegisterSample(value);
}

void ProbePanel::OnFrameRateUpdated(
    float immediateFps,
    float /*averageFps*/)
{
    mFrameRateProbe->RegisterSample(immediateFps);
}

void ProbePanel::OnUpdateToRenderRatioUpdated(
    float immediateURRatio)
{
    mURRatioProbe->RegisterSample(immediateURRatio);
}