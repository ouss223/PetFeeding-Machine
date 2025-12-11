import { ClipLoader } from "react-spinners"; // Import the spinner
import React, { useState, useEffect } from "react";
import { useNavigate } from "react-router-dom";

const ESP32_API = "http://localhost:8180";

function App() {
  const [distance, setDistance] = useState(null);
  const [mode, setMode] = useState(null);
  const [error, setError] = useState(null);
  const [loadingButton1, setLoadingButton1] = useState(false); // Track loading for Button 1
  const [loadingButton2, setLoadingButton2] = useState(false); // Track loading for Button 2
  const navigate = useNavigate();


  const setDeviceMode = async (newMode) => {
    if (newMode === 1) {
      setLoadingButton1(true); // Start loading for Button 1
    } else if (newMode === 2) {
      setLoadingButton2(true); // Start loading for Button 2
    }

    try {
      const response = await fetch(`${ESP32_API}/setMode`, {
        method: "POST",
        headers: {
          "Content-Type": "application/x-www-form-urlencoded",
        },
        body: `mode=${newMode}`,
      });
      if (!response.ok) throw new Error(`Error: ${response.statusText}`);
      setMode(newMode);
      setError(null);
      newMode === 1 ? navigate("/first") : navigate("/second");
    } catch (err) {
      console.error(err);
      setError("Failed to set device mode");
    } finally {
      if (newMode === 1) {
        setLoadingButton1(false); // Stop loading for Button 1
      } else if (newMode === 2) {
        setLoadingButton2(false); // Stop loading for Button 2
      }
    }
  };
  const fetchMode = async () => {
    try {
      const response = await fetch(`${ESP32_API}/getMode`);
      if (!response.ok) throw new Error(`Error: ${response.statusText}`);
      const data = await response.json();
      setMode(data.mode);
      console.log(data);
      setError(null);
    } catch (err) {
      console.error(err);
      setError("Failed to fetch distance");
    }
  };

  useEffect(() => {
    fetchMode();
  }, []);

  return (
    <div className="min-h-screen flex flex-col items-center justify-center bg-gradient-to-r from-blue-500 to-purple-600 text-gray-800 p-6">
      <div className="bg-white shadow-md rounded-lg p-8 w-full max-w-md">
        <h1 className="text-2xl font-bold mb-6 text-center text-blue-600">
          ESP32 Control Panel
        </h1>

        

        <p className="text-lg font-semibold text-center">
          Current Mode:{" "}
          <span className="font-normal">
            {mode === 1
              ? "Normal Mode"
              : mode === 2
              ? "Scheduled Mode"
              : mode == 0
              ? "None"
              : "Loading ..."}
          </span>
        </p>

        {mode == 0 && (<>
            <p className="text-lg font-semibold text-center mt-10">
            Choode Device Mode :
            </p>
        
          <div className="flex justify-around mt-6">
            <button
              onClick={() => setDeviceMode(1)}
              className="bg-blue-500 hover:bg-blue-600 text-white font-semibold py-2 px-4 rounded transition duration-200"
              disabled={loadingButton2 || loadingButton1} // Disable Button 1 while loading
            >
              {loadingButton1 ? (
                <ClipLoader color="#ffffff" size={20} />
              ) : (
                "Normal Mode"
              )}
            </button>
            <button
              onClick={() => setDeviceMode(2)}
              className="bg-green-500 hover:bg-green-600 text-white font-semibold py-2 px-4 rounded transition duration-200"
              disabled={loadingButton2 || loadingButton1} // Disable Button 2 while loading
            >
              {loadingButton2 ? (
                <ClipLoader color="#ffffff" size={20} />
              ) : (
                "Scheduled Mode"
              )}
            </button>
          </div></>
        )}
        {(mode == 1 || mode == 2) && (
          <div className="flex justify-around mt-6">
            <button
              onClick={() => (mode === 1 ? navigate("/first") : navigate("/second"))}
              className="bg-blue-500 hover:bg-blue-600 text-white font-semibold py-2 px-4 rounded transition duration-200"
              disabled={loadingButton2 || loadingButton1} // Disable Button 1 while loading
            >
              {loadingButton1 ? (
                <ClipLoader color="#ffffff" size={20} />
              ) : (
                "dashboard"
              )}
            </button>
            <button
              onClick={() => setMode(0)}
              className="bg-green-500 hover:bg-green-600 text-white font-semibold py-2 px-4 rounded transition duration-200"
              // Disable Button 2 while loading
            >
              {loadingButton2 ? (
                <ClipLoader color="#ffffff" size={20} />
              ) : (
                "Modify Mode"
              )}
            </button>
          </div>
        )}
      </div>
    </div>
  );
}

export default App;
