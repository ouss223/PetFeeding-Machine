import React, { useState, useEffect } from "react";
import FoodStats from "./FoodStats"; // Import the FoodStats component
import { ClipLoader } from "react-spinners"; 
const ESP32_API = "http://localhost:8180";

const SecondComponent = () => {
  const [times, setTimes] = useState([
    [0, 8],
    [0, 14],
    [0, 20],
  ]); // [minute, hour]
  const [error, setError] = useState(null);
  const [loading, setLoading] = useState(true); // Add loading state
  const [editing, setEditing] = useState(false);
  const [loadingSubmit, setLoadingSubmit] = useState(false);
  const foodie = <FoodStats />; // Create an instance of the FoodStats component

  const handleTimeChange = (index, timeIndex, value) => {
    const newTimes = [...times];
    newTimes[index][timeIndex] = Number(value);
    setTimes(newTimes);
  };

  const getDataString = (params) => {
    return Object.keys(params)
      .map(
        (key) => `${encodeURIComponent(key)}=${encodeURIComponent(params[key])}`
      )
      .join("&");
  };

  const handleSubmit = async () => {
    try {
      // Create an object to hold key-value pairs
      setLoadingSubmit(true);
      const scheduleParams = {};
      times.forEach((time, index) => {
        scheduleParams[`schedule[${index}][hour]`] = time[1];
        scheduleParams[`schedule[${index}][minute]`] = time[0];
      });

      // Convert to x-www-form-urlencoded format
      const body = getDataString(scheduleParams);

      const response = await fetch(`${ESP32_API}/setSchedule`, {
        method: "POST",
        headers: {
          "Content-Type": "application/x-www-form-urlencoded",
        },
        body: body,
      });

      if (!response.ok) {
        const errorMessage = await response.text();
        throw new Error(`Error: ${response.statusText} - ${errorMessage}`);
      }

      const result = await response.text();
      console.log(result);
      setEditing(false);
      setLoadingSubmit(false);
      setError(null);
    } catch (err) {
      console.error(err);
      setError("Failed to set schedule");
    }
  };

  const fetchInitialSchedule = async () => {
    try {
      const response = await fetch(`${ESP32_API}/getSchedule`);
      if (!response.ok) {
        const errorMessage = await response.text();
        throw new Error(`Error: ${response.statusText} - ${errorMessage}`);
      }
      const data = await response.json();
      const initialTimes = data.map((item) => [item.minute, item.hour]); // Update based on returned JSON format
      setTimes(initialTimes);
      setError(null);
    } catch (err) {
      console.error(err);
      setError("Failed to fetch initial schedule");
    } finally {
      setLoading(false); // Set loading to false after fetching data
    }
  };

  useEffect(() => {
    fetchInitialSchedule();
  }, []);

  if (loading) {
    return (
      <div className="min-h-screen flex items-center justify-center bg-gradient-to-r from-blue-500 to-purple-600 text-white">
        <div className="text-2xl font-bold">Loading...</div>
      </div>
    );
  } else if (error) {
    return (
      <div className="min-h-screen flex items-center justify-center bg-gradient-to-r from-blue-500 to-purple-600 text-white">
        <div className="text-2xl font-bold">Error...</div>
      </div>
    );
  }

  return (
    <div className=" pt-20 min-h-screen flex flex-col items-center justify-center bg-gradient-to-r from-blue-500  p-6 to-purple-600 text-white">
      <div className="bg-white shadow-md rounded-lg p-8 w-full max-w-md">
        <h1 className="text-2xl font-bold text-center text-blue-600 mb-6">
          Schedule Times
        </h1>

        {editing ? (
          <div>
            {times.map((time, i) => (
              <div key={i} className="mb-4 text-black">
                <h2 className="text-lg font-semibold mb-2 text-gray-700">
                  Time {i + 1}
                </h2>
                <div className="flex gap-4">
                <input
                    type="number"
                    value={time[1]}
                    onChange={(e) => handleTimeChange(i, 1, e.target.value)}
                    className="w-1/2 p-2 border border-gray-300 rounded focus:outline-none focus:ring-2 focus:ring-blue-500"
                    placeholder="Hours"
                  />
                  <input
                    type="number"
                    value={time[0]}
                    onChange={(e) => handleTimeChange(i, 0, e.target.value)}
                    className="w-1/2 p-2 border border-gray-300 rounded focus:outline-none focus:ring-2 focus:ring-blue-500"
                    placeholder="Minutes"
                  />
                 
                </div>
              </div>
            ))}
            <button
              onClick={handleSubmit}
              className="w-full bg-blue-500 hover:bg-blue-600 text-white font-semibold py-2 px-4 rounded transition duration-200 mt-4"
            >
              {loadingSubmit ? <ClipLoader size={20} color={"#fff"} loading={true} /> : "Submit Schedule"}
            </button>
          </div>
        ) : (
          <div>
            {times.map((time, i) => (
              <div key={i} className="mb-4 text-black flex flex-row gap-10">
                <h2 className="text-lg font-semibold mb-2 text-gray-700">
                  Time {i + 1}
                </h2>
                <div className="flex gap-4 text-xl font-semibold text-gray-700">
                  <h3>{time[1]}H</h3>
                  <h3>{time[0]}Min</h3>
                </div>
              </div>
            ))}
            <button
              onClick={() => setEditing(true)}
              className="w-full bg-blue-500 hover:bg-blue-600 text-white font-semibold py-2 px-4 rounded transition duration-200 mt-4"
            >
              Edit schedule
            </button>
          </div>
        )}

        {error && (
          <div className="mt-4 text-center text-red-600 font-semibold">
            {error}
          </div>
        )}
      </div>

      {/* FoodStats Component */}
      <FoodStats />
    </div>
  );
};

export default SecondComponent;
/*
{!editing && <button>Edit schedule</button>

            }

<button
              onClick={handleSubmit}
              className="w-full bg-blue-500 hover:bg-blue-600 text-white font-semibold py-2 px-4 rounded transition duration-200 mt-4"
            >
              Submit Schedule
            </button>

*/
