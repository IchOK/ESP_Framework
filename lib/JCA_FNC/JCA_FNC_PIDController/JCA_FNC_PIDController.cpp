#include <JCA_FNC_PIDController.h>
using namespace JCA::SYS;
using namespace JCA::TAG;

namespace JCA {
  namespace FNC {
    const char *PIDController::ClassName = "PIDController";
    const char *PIDController::SetupTagType = "pidController";
    const char *PIDController::SetupTagProcessUnit = "inUnit";
    const char *PIDController::SetupTagOutputUnit = "outUnit";

    /**
     * @brief Construct a new PIDController::PIDController object
     * 
     * @param _Name Element Name inside the FuncHandler
     * @param _ProcessUnit Unit of the Process Variable (e.g. "°C", "mbar", ...)
     * @param _OutputUnit Unit of the Output Variable (e.g. "%", "rpm", ...)
     */
    PIDController::PIDController(String _Name, String _ProcessUnit, String _OutputUnit)
        : FuncParent(_Name) {
      Debug.println(FLAG_SETUP, false, Name, __func__, "Create");

      // Create Tag-List
      Tags.push_back (new TagFloat ("P", "Proportionaler Verstärkungsfaktor", "", false, TagUsage_T::UseConfig, &P, ""));
      Tags.push_back (new TagFloat ("Ti", "Integrationszeit", "", false, TagUsage_T::UseConfig, &Ti, "s"));
      Tags.push_back (new TagFloat ("Td", "Differentialzeit", "", false, TagUsage_T::UseConfig, &Td, "s"));
      Tags.push_back (new TagFloat ("TdLag", "Abklingkonstante für D-Anteil", "", false, TagUsage_T::UseConfig, &TdLag, "s"));
      Tags.push_back (new TagFloat ("SetpointMin", "Minimaler Sollwert", "", false, TagUsage_T::UseConfig, &SetpointMin, _ProcessUnit));
      Tags.push_back (new TagFloat ("SetpointMax", "Maximaler Sollwert", "", false, TagUsage_T::UseConfig, &SetpointMax, _ProcessUnit));
      Tags.push_back (new TagFloat ("OutputMin", "Minimaler Stellwert", "", false, TagUsage_T::UseConfig, &OutputMin, _OutputUnit));
      Tags.push_back (new TagFloat ("OutputMax", "Maximaler Stellwert", "", false, TagUsage_T::UseConfig, &OutputMax, _OutputUnit));

      Tags.push_back (new TagFloat ("ProcessVar", "Istwert", "", false, TagUsage_T::UseData, &ProcessVar, _ProcessUnit));
      Tags.push_back (new TagFloat ("Setpoint", "Sollwert", "", false, TagUsage_T::UseData, &Setpoint, _ProcessUnit));
      Tags.push_back (new TagBool ("ManualSetpointMode", "Handmodus für den Sollwert", "", false, TagUsage_T::UseData, &ManualSetpointMode, "HAND", "AUTO"));
      Tags.push_back (new TagFloat ("ManualSetpoint", "Manueller Sollwert", "", false, TagUsage_T::UseData, &ManualSetpoint, _ProcessUnit));
      Tags.push_back (new TagBool ("ManualOutputMode", "Handmodus für den Stellwert", "", false, TagUsage_T::UseData, &ManualOutputMode, "HAND", "AUTO"));
      Tags.push_back (new TagFloat ("Value", "Stellwert", "", true, TagUsage_T::UseData, &Value, _OutputUnit));

      P = 1.0;
      Ti = 1.0;
      Td = 0.0;
      TdLag = 1.0;
      SetpointMin = 0.0;
      SetpointMax = 100.0;
      OutputMin = 0.0;
      OutputMax = 100.0;

      Setpoint = 0.0;
      ManualSetpoint = 0.0;
      ProcessVar = 0.0;
      Value = 0.0;
      ManualSetpointMode = false;
      ManualOutputMode = false;

      Proportional = 0.0;
      Integral = 0.0;
      Derivative = 0.0;
      LastError = 0.0;
      LastDerivative = 0.0;
      LastProcessVar = 0.0;
      LastUpdateMillis = millis ();
    }

    void PIDController::update(struct tm &_Time) {
      Debug.println(FLAG_LOOP, false, Name, __func__, "Run");

      // Select active setpoint based on ManualSetpointMode
      float ActiveSetpoint = ManualSetpointMode ? ManualSetpoint : Setpoint;

      // Normalize setpoint and process variable to 0-1 range
      float NormalizedSetpoint = (ActiveSetpoint - SetpointMin) / (SetpointMax - SetpointMin);
      float NormalizedProcessVar = (ProcessVar - SetpointMin) / (SetpointMax - SetpointMin);
      float NormalizedOutput;

      unsigned long CurrentMillis = millis();
      float DeltaTime = (float)(CurrentMillis - LastUpdateMillis) / 1000.0; // Time difference in seconds

      // Calculate the error
      float Error = NormalizedSetpoint - NormalizedProcessVar;

      // Proportional
      Proportional = P * Error;

      // Derivative with decay
      if (Td > 0) {
        float ActDerivative = Td * (NormalizedProcessVar - LastProcessVar) / DeltaTime;
        if (TdLag > 0) {
          Derivative = Derivative + (ActDerivative - Derivative) * (DeltaTime / TdLag);
        } else {
          Derivative = ActDerivative;
        }
      }

      if (ManualOutputMode) {
        //--------------------------------
        // Manual mode for Output
        //--------------------------------
        Debug.println (FLAG_LOOP, false, Name, __func__, "Manual mode for Output active");

        // Callculate normalized output
        NormalizedOutput = (Value - OutputMin) / (OutputMax - OutputMin);

      } else {
        //--------------------------------
        // Auto mode for Output
        //--------------------------------
        if (Ti > 0) {
          Integral += Error * DeltaTime / Ti;
        } else {
          Integral = 0.0;
        }

        // PID Output (normalized)
        NormalizedOutput = Proportional + Integral - Derivative;
      }

      //----------------------------
      // Limit the output to the range [0, 1]
      // and adjust the Integral value if necessary
      //----------------------------
      if (NormalizedOutput > 1.0) {
        NormalizedOutput = 1.0;
        if (Ti > 0) {
          Integral = 1.0 - Proportional + Derivative;
        }
      } else if (NormalizedOutput < 0.0) {
        NormalizedOutput = 0.0;
        if (Ti > 0) {
          Integral = 0.0 - Proportional + Derivative;
        }
      }

      // Calculate the final output value
      Value = NormalizedOutput * (OutputMax - OutputMin) + OutputMin;

      LastProcessVar = NormalizedProcessVar;
      LastDerivative = Derivative;
      LastError = Error;
      LastUpdateMillis = CurrentMillis;

      Debug.print(FLAG_LOOP, false, Name, __func__, "Output: ");
      Debug.println(FLAG_LOOP, false, Name, __func__, Value);
    }

    /**
     * @brief Adds the creation method to the Function-Handler
     *
     * @param _Handler Function Handler
     */
    void PIDController::AddToHandler(JCA::IOT::FuncHandler &_Handler) {
      _Handler.FunctionList.insert(std::pair<String, std::function<bool(JsonObject, JsonObject, std::vector<JCA::FNC::FuncParent *> &, std::map<String, void *>)>>(SetupTagType, Create));
    }

    /**
     * @brief Create a new Instanz of the Class using the JSON-Configdata and add it to the Functions-List
     *
     * @param _Setup Object contains the creation data
     * @param _Log Logging-Object for Debug after creation
     * @param _Functions List of Function to add the Instanz to
     * @param _Hardware List of knowen Hardware-References
     * @return true
     * @return false
     */
    bool PIDController::Create(JsonObject _Setup, JsonObject _Log, std::vector<FuncParent *> &_Functions, std::map<String, void *> _Hardware) {
      Debug.println (FLAG_SETUP, true, ClassName, __func__, "Start");
      bool Done = true;
      JsonObject Log = _Log[SetupTagType].to<JsonObject> ();

      String Name = GetSetupValueString (JCA_IOT_FUNCHANDLER_SETUP_NAME, Done, _Setup, Log);
      String ProcessUnit = GetSetupValueString (SetupTagProcessUnit, Done, _Setup, Log);
      String OutputUnit = GetSetupValueString (SetupTagOutputUnit, Done, _Setup, Log);

      if (Done) {
        _Functions.push_back (new PIDController (Name, ProcessUnit, OutputUnit));
        Log["done"] = Name + "(Process:" + ProcessUnit + " Output:" + OutputUnit + ")";
        Debug.println (FLAG_SETUP, true, ClassName, __func__, "Done");
      }
      return Done;
    }
  }
}