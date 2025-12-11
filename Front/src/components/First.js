import React, { useEffect } from "react";
import { useState } from "react";
import FoodStats from "./FoodStats";

const ESP32_API = "http://localhost:8180";

const First = () => {
  const [stockFood, setStockFood] = useState(9);
  const [stockWater, setstockWater] = useState(9);
  const [servedFood, setServedFood] = useState(9);
  const [servedWater, setservedWater] = useState(9);
  const [error, setError] = useState(null);
  const [loading, setLoading] = useState(true); // Add loading state

  const fetchInfos = async () => {
    try {
      const response = await fetch(`${ESP32_API}/getInfos`);
      if (!response.ok) throw new Error(`Error: ${response.statusText}`);
      const data = await response.json();
      setStockFood(data.stockFood);
      setstockWater(data.stockWater);
      setServedFood(data.servedFood);
      setservedWater(data.servedWater);
      setError(null); // clear any previous errors
    } catch (err) {
      console.error(err);
      setError("Failed to fetch data");
    } finally {
      setLoading(false); // Set loading to false after fetching data
    }
  };

  useEffect(() => {
    fetchInfos();
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
    <div className="pt-20 min-h-screen bg-gradient-to-r from-blue-500 to-purple-600 flex flex-col items-center justify-center text-white p-6">
      <div className="bg-white shadow-xl rounded-xl p-8 w-full max-w-md text-center space-y-6">
        <h1 className="text-3xl font-extrabold text-blue-600 mb-4">
          Pet Feeding Tracker
        </h1>

        <div className="space-y-3">
          <h2 className="text-xl font-semibold text-gray-700">
            Stock Food:{" "}
            <span className="font-normal text-gray-900">{stockFood}%</span>
          </h2>
          <h2 className="text-xl font-semibold text-gray-700">
            Stock Water:{" "}
            <span className="font-normal text-gray-900">{stockWater}%</span>
          </h2>
          <h2 className="text-xl font-semibold text-gray-700">
            Served Food:{" "}
            <span className="font-normal text-gray-900">{servedFood}%</span>
          </h2>
          <h2 className="text-xl font-semibold text-gray-700">
            Served Water:{" "}
            <span className="font-normal text-gray-900">{servedWater}%</span>
          </h2>
        </div>

        {error && <p className="text-red-600 font-medium">{error}</p>}
      </div>

      {/* FoodStats component */}
      <FoodStats />
    </div>
  );
};

export default First;
