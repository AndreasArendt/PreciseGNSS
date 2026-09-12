#include "rinex/RinexObsParser/RinexObsParser.hpp"

#include <iomanip>
#include <sstream>
#include <stdexcept>

void require(bool condition) {
  if (!condition)
    throw std::runtime_error("RINEX signal identity regression");
}

int main() {
  std::ostringstream input;
  input << "     3.04           OBSERVATION DATA    M                   RINEX VERSION / TYPE\n";
  std::string types = "G    8 C2S C2L L2S L2L D2S D2L S2S S2L";
  types.resize(60, ' ');
  input << types << "SYS / # / OBS TYPES\n"
        << std::string("DBHZ") + std::string(56, ' ') + "SIGNAL STRENGTH UNIT\n"
        << "                                                            END OF HEADER\n"
        << "> 2026 09 12 00 00  0.0000000  0  1\nG01";
  int field = 0;
  for (double value : {21000001., 21000002., 100001., 100002., -101., -102., 41., 42.}) {
    input << std::fixed << std::setprecision(3) << std::setw(14) << value;
    input << (field == 0 ? "05" : field == 2 ? "79" : "  ");
    ++field;
  }
  input << "\n> 2026 09 12 00 00  1.0000000  0  1\nG01";
  for (int index = 0; index < 8; ++index) {
    if (index == 0 || index == 7)
      input << std::string(16, ' ');
    else
      input << std::setw(14) << 100.0 << "  ";
  }
  input << '\n';

  std::vector<ObservationEpoch> epochs;
  RinexObsParser parser(epochs);
  std::istringstream stream(input.str());
  parser.Parse(stream);
  require(epochs.size() == 2);
  require(parser.SignalStrengthUnit() == "DBHZ");
  const SignalId s{ObservationBand::Band_2, ObservationAttribute::Channel_S};
  const SignalId l{ObservationBand::Band_2, ObservationAttribute::Channel_L};
  const auto &obs = epochs.at(0).satellites.at(0);
  require(obs.CodeObservations.at(s).quality.lli == 0);
  require(obs.CodeObservations.at(s).quality.ssi == 5);
  require(obs.PhaseObservations.at(s).quality.lli == 7);
  require(obs.PhaseObservations.at(s).quality.ssi == 9);
  require(!obs.CodeObservations.at(l).quality.lli);
  require(!obs.CodeObservations.at(l).quality.ssi);
  require(obs.CodeObservations.size() == 2);
  require(obs.PhaseObservations.size() == 2);
  require(obs.DopplerObservations.size() == 2);
  require(obs.SnrObservations.size() == 2);
  require(obs.CodeObservations.at(s).pseudorange_m == 21000001.);
  require(obs.CodeObservations.at(l).pseudorange_m == 21000002.);
  require(obs.PhaseObservations.at(s).carrier_phase_cycles == 100001.);
  require(obs.PhaseObservations.at(l).carrier_phase_cycles == 100002.);
  require(obs.DopplerObservations.at(s).doppler_hz == -101.);
  require(obs.DopplerObservations.at(l).doppler_hz == -102.);
  require(obs.SnrObservations.at(s).snr == 41.);
  require(obs.SnrObservations.at(l).snr == 42.);
  const auto &missing = epochs.at(1).satellites.at(0);
  require(!missing.CodeObservations.contains(s));
  require(missing.CodeObservations.contains(l));
  require(missing.SnrObservations.contains(s));
  require(!missing.SnrObservations.contains(l));

  // Missing unit header and omitted trailing indicator columns remain absent.
  std::string noUnit = input.str();
  const auto unitStart = noUnit.find("DBHZ");
  noUnit.erase(unitStart, noUnit.find('\n', unitStart) - unitStart + 1);
  const auto epochEnd = noUnit.find("\n> 2026", noUnit.find("\n> 2026") + 1);
  noUnit.erase(epochEnd - 2, 2);
  epochs.clear();
  std::istringstream noUnitStream(noUnit);
  parser.Parse(noUnitStream);
  require(!parser.SignalStrengthUnit());
  require(!epochs.at(0).satellites.at(0).SnrObservations.at(l).quality.ssi);
  require(!epochs.at(0).satellites.at(0).SnrObservations.at(l).quality.lli);
}
