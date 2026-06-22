"""
statistics_calculator.py

Object-oriented implementation of basic descriptive statistics
(mean, median, mode) on a list of integers.

Paradigm: Object-Oriented (Python)
- A StatisticsCalculator class encapsulates the dataset (state) and
  the operations performed on it (behavior) in a single unit.
- Each statistic is its own method, demonstrating encapsulation: the
  caller never has to manually sort the list or build a frequency
  table -- those details are hidden inside the class.
- A Python dict (hash map) is used to tally value frequencies for
  the mode calculation, and Python's built-in list/sort facilities
  are used for the median.

Run:  python3 statistics_calculator.py
"""

from typing import List, Tuple


class StatisticsCalculator:
    """Encapsulates a dataset of integers and computes descriptive
    statistics (mean, median, mode) on demand.
    """

    def __init__(self, data: List[int]) -> None:
        """Store a defensive copy of the data so external code can't
        mutate the calculator's internal state after construction.
        """
        if not data:
            raise ValueError("StatisticsCalculator requires a non-empty list of integers.")
        self._data: List[int] = list(data)

    # ------------------------------------------------------------------
    # Public read-only access to the underlying dataset
    # ------------------------------------------------------------------
    @property
    def data(self) -> List[int]:
        """Return a copy of the stored data (keeps internal state private)."""
        return list(self._data)

    # ------------------------------------------------------------------
    # Mean
    # ------------------------------------------------------------------
    def calculate_mean(self) -> float:
        """Return the arithmetic mean of the dataset."""
        return sum(self._data) / len(self._data)

    # ------------------------------------------------------------------
    # Median
    # ------------------------------------------------------------------
    def calculate_median(self) -> float:
        """Return the median of the dataset (average of the two middle
        values when the count is even).
        """
        sorted_data = sorted(self._data)
        n = len(sorted_data)
        mid = n // 2

        if n % 2 == 1:
            return float(sorted_data[mid])
        return (sorted_data[mid - 1] + sorted_data[mid]) / 2.0

    # ------------------------------------------------------------------
    # Mode
    # ------------------------------------------------------------------
    def calculate_mode(self) -> Tuple[List[int], int]:
        """Return a tuple of (modes, frequency) where `modes` is the
        sorted list of every value tied for the highest frequency, and
        `frequency` is that highest frequency.
        """
        frequency_table = {}
        for value in self._data:
            frequency_table[value] = frequency_table.get(value, 0) + 1

        highest_freq = max(frequency_table.values())
        modes = sorted(
            value for value, count in frequency_table.items()
            if count == highest_freq
        )
        return modes, highest_freq

    # ------------------------------------------------------------------
    # Convenience: compute everything at once
    # ------------------------------------------------------------------
    def summary(self) -> dict:
        """Return all three statistics together as a dictionary."""
        modes, freq = self.calculate_mode()
        return {
            "mean": self.calculate_mean(),
            "median": self.calculate_median(),
            "mode": modes,
            "mode_frequency": freq,
        }

    def __repr__(self) -> str:
        return f"StatisticsCalculator(data={self._data})"


def main() -> None:
    dataset = [4, 8, 15, 16, 23, 42, 4, 8, 15, 16, 8]

    calculator = StatisticsCalculator(dataset)

    print(f"Dataset: {calculator.data}\n")

    mean_value = calculator.calculate_mean()
    median_value = calculator.calculate_median()
    modes, freq = calculator.calculate_mode()

    print(f"Mean:   {mean_value:.4f}")
    print(f"Median: {median_value:.4f}")
    print(f"Mode(s) (frequency = {freq}): {modes}")


if __name__ == "__main__":
    main()
