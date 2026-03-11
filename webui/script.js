let chartInstance = null;

async function runAction(action) {
  const output = document.getElementById("output");
  const info = document.getElementById("algoInfo");
  const chart = document.getElementById("chart");
  output.innerHTML = "⏳ Processing...";
  info.classList.add("hidden");
  chart.classList.add("hidden");

  const urls = {
    generate: "/generate",
    label: "/label",
    train: "/train",
    predict: "/predict"
  };

  try {
    const res = await fetch(urls[action], { method: "POST" });
    const data = await res.json();

    if (data.error) {
      output.innerHTML = data.error;
      return;
    }

    output.innerHTML = data.message;

    if (data.algorithm) {
      info.innerHTML = `<b>${data.algorithm}</b><br>${data.description}<br><small>${data.complexity}</small>`;
      info.classList.remove("hidden");

      drawChart(data.algorithm, data.score);
    }
  } catch {
    output.innerHTML = "⚠️ Server error.";
  }
}

async function generateTestArray() {
  const size = document.getElementById("arraySize").value;
  const output = document.getElementById("output");
  const display = document.getElementById("arrayDisplay");

  if (!size || size <= 0) {
    output.innerHTML = "⚠️ Enter a valid size.";
    return;
  }

  output.innerHTML = `⏳ Generating random array...`;
  const res = await fetch("/generate_test", {
    method: "POST",
    headers: {"Content-Type": "application/json"},
    body: JSON.stringify({ size })
  });
  const data = await res.json();
  output.innerHTML = data.message;
  display.classList.remove("hidden");
  display.innerHTML = `<b>Generated Array:</b><br>${data.array.join(", ")}`;
}

async function manualTestArray() {
  const input = document.getElementById("manualArray").value.trim();
  const output = document.getElementById("output");
  const display = document.getElementById("arrayDisplay");

  if (!input) {
    output.innerHTML = "⚠️ Please enter an array.";
    return;
  }

  output.innerHTML = "⏳ Submitting array...";
  const res = await fetch("/manual_test", {
    method: "POST",
    headers: {"Content-Type": "application/json"},
    body: JSON.stringify({ array: input })
  });
  const data = await res.json();
  output.innerHTML = data.message || data.error;
  if (data.array) {
    display.classList.remove("hidden");
    display.innerHTML = `<b>Manual Array:</b><br>${data.array.join(", ")}`;
  }
}

function drawChart(algorithm, score) {
  const ctx = document.getElementById("chart").getContext("2d");
  document.getElementById("chart").classList.remove("hidden");

  const labels = ["BubbleSort", "InsertionSort", "SelectionSort", "ShellSort", "QuickSort", "CountingSort", "RadixSort"];
  const values = [1, 2, 2, 3, 5, 6, 6];
  const colors = labels.map(l => l === algorithm ? "#2ea043" : "#30363d");

  if (chartInstance) chartInstance.destroy();

  chartInstance = new Chart(ctx, {
    type: "bar",
    data: {
      labels,
      datasets: [{
        label: "Efficiency Score (Higher = Better)",
        data: values,
        backgroundColor: colors
      }]
    },
    options: {
      scales: { y: { beginAtZero: true } },
      plugins: { legend: { display: false } }
    }
  });
}
