import json
import platform
import shutil
import subprocess
import sys
from pathlib import Path


def llvm_tool(name):
    if platform.system() == "Darwin":
        return subprocess.check_output(["xcrun", "--find", name], text=True).strip()
    tool = shutil.which(name)
    if tool is None:
        raise RuntimeError(f"Install LLVM tools: {name} was not found")
    return tool


def main():
    root = Path(__file__).resolve().parents[1]
    build = Path(sys.argv[1]).resolve()
    for profile in build.glob("coverage-*.profraw"):
        profile.unlink()
    subprocess.run(["ctest", "--test-dir", str(build), "--output-on-failure"], check=True)
    profiles = sorted(build.glob("coverage-*.profraw"))
    if not profiles:
        raise RuntimeError("No profiles were generated; configure with POSTAL_COVERAGE=ON")
    merged = build / "coverage.profdata"
    subprocess.run([llvm_tool("llvm-profdata"), "merge", "-sparse",
                    *map(str, profiles), "-o", str(merged)], check=True)
    production = {str(path.resolve()) for pattern in ("*.cpp", "*.h") for path in root.glob(pattern)}
    totals = {}
    for executable in ("postal_tests", "postal"):
        raw = subprocess.check_output([llvm_tool("llvm-cov"), "export", str(build / executable),
                                       f"-instr-profile={merged}"], text=True)
        for data in json.loads(raw)["data"]:
            for item in data["files"]:
                name = str(Path(item["filename"]).resolve())
                if name in production:
                    current = item["summary"]["lines"]
                    if name not in totals or current["covered"] > totals[name]["covered"]:
                        totals[name] = current
    expected_sources = {str(path.resolve()) for path in root.glob("*.cpp")}
    missing = expected_sources - totals.keys()
    if missing:
        raise RuntimeError("Production sources missing from coverage: " + ", ".join(sorted(missing)))
    count = sum(item["count"] for item in totals.values())
    covered = sum(item["covered"] for item in totals.values())
    percent = 100 * covered / count if count else 0
    summary = {"covered": covered, "count": count, "percent": percent, "files": totals}
    (build / "coverage-summary.json").write_text(json.dumps(summary, indent=2) + "\n")
    print(f"Production line coverage: {covered}/{count} = {percent:.2f}%")
    if percent <= 90:
        raise SystemExit("Coverage must be greater than 90%")


if __name__ == "__main__":
    main()
