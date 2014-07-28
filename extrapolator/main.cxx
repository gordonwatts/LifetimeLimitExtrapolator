#include <iostream>
#include <string>
#include <functional>
#include <map>

#include "TRandom.h"
#include "TString.h"
#include "TFile.h"
#include "TH1F.h"

using namespace std;

int main()
{
	// Config parameters (shoudl be loaded from a file).
	string dataset = "900_150";
	double generated_ctau = 1.3; // meters (from the note appendix)

	// Functions we can use to generate various things
	double ctau = generated_ctau;
	function<pair<double, double> (void) > generate_lifetime_dual_exp = [ctau]() { return make_pair(gRandom->Exp(ctau), gRandom->Exp(ctau)); };

	// Choose what we will use to do the generation.
	function<pair<double, double>(void)> generate_lifetime = generate_lifetime_dual_exp;

	// Output file
	auto fname = TString("../results_") + dataset + ".root";
	auto hist_output = TFile::Open(fname, "RECREATE");

	// Histograms to save.
	auto h_ctau1 = new TH1F("ctau_1", "ctau of vpion 1", 5000, 0.0, 30.0);
	auto h_ctau2 = (TH1F*)h_ctau1->Clone("ctau_2");
	h_ctau2->SetTitle("ctau of vpion 2");

	// Run the toy
	const int toy_runs = 1000;
	for (int i_toy = 0; i_toy < toy_runs; i_toy++) {
		// We need the pT and the lifetimes of the objects.
		auto lt = generate_lifetime();
		h_ctau1->Fill(lt.first);
		h_ctau2->Fill(lt.second);
	}

	// Make sure everything is correctly saved.
	hist_output->Write();
	hist_output->Close();
	return 0;
}