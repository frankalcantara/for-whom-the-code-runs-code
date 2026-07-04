#!/usr/bin/env bash

# WSL/Linux/Windows compile check for the complete companion repository.
# The script probes the available C++23 toolchains once, then compiles each
# source file with the first compiler/standard-library pair that supports the
# features used by that file.

set -u
set -o pipefail

export PATH="/usr/bin:/bin:$PATH"

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$SCRIPT_DIR"
BUILD_DIR="${BUILD_DIR:-$ROOT_DIR/.compile-check}"
REPORT="${REPORT:-$ROOT_DIR/compilation-report.txt}"
LLVM_VERSION="${LLVM_VERSION:-}"
GCC_VERSION="${GCC_VERSION:-}"
TOOLCHAIN_ORDER="${TOOLCHAIN_ORDER:-clang-libc++ clang-libc++-experimental clang-libstdc++ clang-libstdc++-tbb gcc gcc-tbb msvc}"

COMMON_CXXFLAGS=(-std=c++23 -O2 -Wall -Wextra -pthread)

declare -a TOOLCHAIN_IDS=()
declare -A TOOLCHAIN_LABEL=()
declare -A TOOLCHAIN_COMPILER=()
declare -A TOOLCHAIN_KIND=()
declare -A TOOLCHAIN_CXXFLAGS=()
declare -A TOOLCHAIN_LDFLAGS=()
declare -A TOOLCHAIN_CAPS=()
declare -A TOOLCHAIN_SELECTED_COUNT=()

usage() {
    cat <<'EOF'
Usage: bash compiling.sh [options]

Options:
  --clean          Remove the build directory before compiling.
  --preflight     Probe toolchains and stop before compiling sources.
  --help          Show this help.

Environment:
  LLVM_VERSION    Prefer clang++-$LLVM_VERSION when present.
  GCC_VERSION     Prefer g++-$GCC_VERSION when present.
  MSVC_VSDEVCMD   Path to VsDevCmd.bat when auto-discovery is not enough.
  BUILD_DIR       Output directory (default: .compile-check).
  REPORT          Report file (default: compilation-report.txt).
  TOOLCHAIN_ORDER Space-separated candidate order.
EOF
}

CLEAN=0
PREFLIGHT_ONLY=0

while [ "$#" -gt 0 ]; do
    case "$1" in
        --clean)
            CLEAN=1
            ;;
        --preflight|--preflight-only)
            PREFLIGHT_ONLY=1
            ;;
        --help|-h)
            usage
            exit 0
            ;;
        *)
            printf 'Unknown option: %s\n\n' "$1" >&2
            usage >&2
            exit 2
            ;;
    esac
    shift
done

log() {
    printf '%s\n' "$*" | tee -a "$REPORT"
}

join_by_space() {
    printf '%s' "$*"
}

first_diagnostic() {
    local error_file="$1"
    local line

    line="$(
        grep -E -m 1 '(^|: )(fatal error|error|warning)(:| )|undefined reference|ld: |collect2:' "$error_file" 2>/dev/null || true
    )"
    if [ -n "$line" ]; then
        printf '%s\n' "$line"
        return
    fi

    grep -m 1 '[^[:space:]]' "$error_file" 2>/dev/null || true
}

compiler_version_number() {
    local compiler="$1"
    local base
    base="$(basename "$compiler")"
    if [[ "$base" =~ clang\+\+-([0-9]+)$ ]]; then
        printf '%s\n' "${BASH_REMATCH[1]}"
        return
    fi

    "$compiler" --version 2>/dev/null |
        sed -nE 's/.*version ([0-9]+).*/\1/p' |
        head -n 1
}

pick_clang() {
    local version
    if [ -n "$LLVM_VERSION" ] && command -v "clang++-$LLVM_VERSION" >/dev/null 2>&1; then
        command -v "clang++-$LLVM_VERSION"
        return
    fi

    for version in 23 22 21 20 19 18 17 16; do
        if command -v "clang++-$version" >/dev/null 2>&1; then
            command -v "clang++-$version"
            return
        fi
    done

    command -v clang++ 2>/dev/null || true
}

pick_gcc() {
    local version
    if [ -n "$GCC_VERSION" ] && command -v "g++-$GCC_VERSION" >/dev/null 2>&1; then
        command -v "g++-$GCC_VERSION"
        return
    fi

    for version in 15 14 13 12 11; do
        if command -v "g++-$version" >/dev/null 2>&1; then
            command -v "g++-$version"
            return
        fi
    done

    command -v g++ 2>/dev/null || true
}

multiarch_name() {
    if command -v dpkg-architecture >/dev/null 2>&1; then
        dpkg-architecture -qDEB_HOST_MULTIARCH 2>/dev/null || true
    fi
}

find_libcxx_flags() {
    local compiler="$1"
    local version="${LLVM_VERSION:-}"
    local multiarch
    local include_dir=""
    local config_dir=""
    local lib_dir=""
    local dir
    local cxxflags="-stdlib=libc++"
    local ldflags=""

    if [ -z "$version" ]; then
        version="$(compiler_version_number "$compiler")"
    fi

    local include_candidates=()
    if [ -n "$version" ]; then
        include_candidates+=("/usr/lib/llvm-$version/include/c++/v1")
    fi
    include_candidates+=("/usr/include/c++/v1")

    for dir in "${include_candidates[@]}"; do
        if [ -f "$dir/vector" ]; then
            include_dir="$dir"
            break
        fi
    done

    multiarch="$(multiarch_name)"
    local config_candidates=()
    if [ -n "$multiarch" ]; then
        config_candidates+=("/usr/include/$multiarch/c++/v1")
        if [ -n "$version" ]; then
            config_candidates+=("/usr/lib/llvm-$version/include/$multiarch/c++/v1")
        fi
    fi
    config_candidates+=("/usr/include/x86_64-linux-gnu/c++/v1")

    for dir in "${config_candidates[@]}"; do
        if [ -f "$dir/__config_site" ]; then
            config_dir="$dir"
            break
        fi
    done

    if [ -n "$include_dir" ]; then
        cxxflags="$cxxflags -nostdinc++"
        if [ -n "$config_dir" ]; then
            cxxflags="$cxxflags -isystem $config_dir"
        fi
        cxxflags="$cxxflags -isystem $include_dir"
    fi

    local lib_candidates=()
    if [ -n "$version" ]; then
        lib_candidates+=("/usr/lib/llvm-$version/lib")
    fi
    if [ -n "$multiarch" ]; then
        lib_candidates+=("/usr/lib/$multiarch")
    fi
    lib_candidates+=("/usr/lib/x86_64-linux-gnu" "/usr/lib")

    for dir in "${lib_candidates[@]}"; do
        if [ -e "$dir/libc++.so" ] || [ -e "$dir/libc++.a" ]; then
            lib_dir="$dir"
            break
        fi
    done

    if [ -n "$lib_dir" ]; then
        ldflags="-L $lib_dir -Wl,-rpath,$lib_dir"
        if [ -e "$lib_dir/libc++abi.so" ] || [ -e "$lib_dir/libc++abi.a" ]; then
            ldflags="$ldflags -lc++abi"
        fi
    fi

    printf '%s\n%s\n' "$cxxflags" "$ldflags"
}

compiler_accepts_flag() {
    local compiler="$1"
    local flag="$2"
    local probe_dir="$BUILD_DIR/feature-tests"
    local source="$probe_dir/accepts-$(printf '%s' "$flag" | tr -c '[:alnum:]' '_').cpp"

    mkdir -p "$probe_dir"
    printf 'int main() { return 0; }\n' > "$source"
    "$compiler" "$flag" "$source" -o "$probe_dir/accepts-flag" >/dev/null 2>&1
}

add_toolchain() {
    local id="$1"
    local label="$2"
    local compiler="$3"
    local cxxflags="$4"
    local ldflags="$5"
    local kind="${6:-posix}"

    TOOLCHAIN_IDS+=("$id")
    TOOLCHAIN_LABEL["$id"]="$label"
    TOOLCHAIN_COMPILER["$id"]="$compiler"
    TOOLCHAIN_KIND["$id"]="$kind"
    TOOLCHAIN_CXXFLAGS["$id"]="$cxxflags"
    TOOLCHAIN_LDFLAGS["$id"]="$ldflags"
    TOOLCHAIN_CAPS["$id"]=""
    TOOLCHAIN_SELECTED_COUNT["$id"]=0
}

path_to_windows() {
    local path="$1"
    local drive
    local rest

    if command -v cygpath >/dev/null 2>&1; then
        cygpath -w "$path"
        return
    fi

    if command -v wslpath >/dev/null 2>&1; then
        wslpath -w "$path"
        return
    fi

    if [[ "$path" =~ ^/mnt/([A-Za-z])/(.*)$ ]]; then
        drive="${BASH_REMATCH[1]}"
        rest="${BASH_REMATCH[2]//\//\\}"
        printf '%s:\\%s\n' "$(printf '%s' "$drive" | tr '[:lower:]' '[:upper:]')" "$rest"
        return
    fi

    if [[ "$path" =~ ^/([A-Za-z])/(.*)$ ]]; then
        drive="${BASH_REMATCH[1]}"
        rest="${BASH_REMATCH[2]//\//\\}"
        printf '%s:\\%s\n' "$(printf '%s' "$drive" | tr '[:lower:]' '[:upper:]')" "$rest"
        return
    fi

    printf '%s\n' "$path"
}

path_to_bash() {
    local path="$1"
    local drive
    local rest

    if [[ "$path" != *\\* && ! "$path" =~ ^[A-Za-z]: ]]; then
        printf '%s\n' "$path"
        return
    fi

    if command -v cygpath >/dev/null 2>&1; then
        cygpath -u "$path"
        return
    fi

    if command -v wslpath >/dev/null 2>&1; then
        wslpath -u "$path"
        return
    fi

    drive="${path:0:1}"
    rest="${path:2}"
    rest="${rest//\\//}"
    drive="$(printf '%s' "$drive" | tr '[:upper:]' '[:lower:]')"
    printf '/mnt/%s/%s\n' "$drive" "$rest"
}

find_vsdevcmd() {
    local vswhere
    local install
    local install_bash
    local candidate
    local -a vswhere_candidates=()

    if [ -n "${MSVC_VSDEVCMD:-}" ]; then
        printf '%s\n' "$(path_to_windows "$MSVC_VSDEVCMD")"
        return
    fi

    if command -v vswhere.exe >/dev/null 2>&1; then
        vswhere_candidates+=("$(command -v vswhere.exe)")
    fi

    for candidate in \
        "/mnt/c/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe" \
        "/c/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe"; do
        if [ -f "$candidate" ]; then
            vswhere_candidates+=("$candidate")
        fi
    done

    for vswhere in "${vswhere_candidates[@]}"; do
        install="$(
            "$vswhere" -latest -products '*' \
                -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 \
                -property installationPath 2>/dev/null |
                tr -d '\r' |
                head -n 1
        )"
        if [ -z "$install" ]; then
            continue
        fi

        install_bash="$(path_to_bash "$install")"
        candidate="$install_bash/Common7/Tools/VsDevCmd.bat"
        if [ -f "$candidate" ]; then
            path_to_windows "$candidate"
            return
        fi
    done
}

msvc_compile() {
    local vcvars="$1"
    local source="$2"
    local output="$3"
    local error_file="$4"
    local flags="$5"
    local win_source
    local win_output
    local win_object
    local bat_file
    local win_bat

    if ! command -v cmd.exe >/dev/null 2>&1; then
        printf 'cmd.exe is required to invoke MSVC from this shell.\n' >"$error_file"
        return 1
    fi

    win_source="$(path_to_windows "$source")"
    win_output="$(path_to_windows "$output.exe")"
    win_object="$(path_to_windows "$output.obj")"
    bat_file="$output.msvc.bat"
    win_bat="$(path_to_windows "$bat_file")"

    cat > "$bat_file" <<EOF
@echo off
call "$vcvars" -arch=x64 -host_arch=x64 >nul
if errorlevel 1 exit /b %errorlevel%
cl /nologo $flags /Fe"$win_output" /Fo"$win_object" "$win_source"
exit /b %errorlevel%
EOF

    cmd.exe //s //c "$win_bat" >"$error_file" 2>&1
}

msvc_version() {
    local vcvars="$1"
    local probe_dir="$BUILD_DIR/feature-tests/msvc"
    local bat_file
    local win_bat

    if ! command -v cmd.exe >/dev/null 2>&1; then
        return
    fi

    mkdir -p "$probe_dir" || return
    bat_file="$probe_dir/msvc-version.bat"
    win_bat="$(path_to_windows "$bat_file")"

    cat > "$bat_file" <<EOF
@echo off
call "$vcvars" -arch=x64 -host_arch=x64 >nul
if errorlevel 1 exit /b %errorlevel%
cl 2>&1
EOF

    cmd.exe //s //c "$win_bat" 2>/dev/null |
        tr -d '\r' |
        grep -i -m 1 'compiler version'
}

discover_toolchains() {
    local clang
    local gcc
    local msvc
    local libcxx_flags
    local libcxx_cxxflags
    local libcxx_ldflags

    clang="$(pick_clang)"
    if [ -n "$clang" ]; then
        mapfile -t libcxx_flags < <(find_libcxx_flags "$clang")
        libcxx_cxxflags="${libcxx_flags[0]}"
        libcxx_ldflags="${libcxx_flags[1]}"

        add_toolchain \
            "clang-libc++" \
            "Clang with libc++" \
            "$clang" \
            "$(join_by_space "${COMMON_CXXFLAGS[@]}") $libcxx_cxxflags" \
            "$libcxx_ldflags"

        if compiler_accepts_flag "$clang" "-fexperimental-library"; then
            add_toolchain \
                "clang-libc++-experimental" \
                "Clang with libc++ and -fexperimental-library" \
                "$clang" \
                "$(join_by_space "${COMMON_CXXFLAGS[@]}") -fexperimental-library $libcxx_cxxflags" \
                "$libcxx_ldflags"
        fi

        add_toolchain \
            "clang-libstdc++" \
            "Clang with libstdc++" \
            "$clang" \
            "$(join_by_space "${COMMON_CXXFLAGS[@]}")" \
            ""

        add_toolchain \
            "clang-libstdc++-tbb" \
            "Clang with libstdc++ and TBB" \
            "$clang" \
            "$(join_by_space "${COMMON_CXXFLAGS[@]}")" \
            "-ltbb"
    fi

    gcc="$(pick_gcc)"
    if [ -n "$gcc" ]; then
        add_toolchain \
            "gcc" \
            "GCC with libstdc++" \
            "$gcc" \
            "$(join_by_space "${COMMON_CXXFLAGS[@]}")" \
            ""

        add_toolchain \
            "gcc-tbb" \
            "GCC with libstdc++ and TBB" \
            "$gcc" \
            "$(join_by_space "${COMMON_CXXFLAGS[@]}")" \
            "-ltbb"
    fi

    msvc="$(find_vsdevcmd || true)"
    if [ -n "$msvc" ]; then
        add_toolchain \
            "msvc" \
            "MSVC cl.exe via VsDevCmd.bat" \
            "$msvc" \
            "/std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus" \
            "" \
            "msvc"
    fi
}

feature_source() {
    local feature="$1"
    local file="$2"

    case "$feature" in
        print)
            cat > "$file" <<'CPP'
#include <print>
int main() {
    std::println("{}", 42);
}
CPP
            ;;
        format)
            cat > "$file" <<'CPP'
#include <format>
#include <string>
int main() {
    return std::format("{:>4}", 7) == "   7" ? 0 : 1;
}
CPP
            ;;
        ranges)
            cat > "$file" <<'CPP'
#include <algorithm>
#include <ranges>
#include <vector>
int main() {
    std::vector<int> values{3, 1, 2};
    std::ranges::sort(values);
    auto even = values | std::views::filter([](int x) { return x % 2 == 0; });
    return *even.begin() == 2 ? 0 : 1;
}
CPP
            ;;
        ranges23)
            cat > "$file" <<'CPP'
#include <ranges>
#include <vector>
int main() {
    std::vector<int> values{1, 2, 3, 4, 5, 6};
    auto even_squares = values
        | std::views::filter([](int x) { return x % 2 == 0; })
        | std::views::transform([](int x) { return x * x; })
        | std::views::take(2)
        | std::ranges::to<std::vector>();

    long long weighted = 0;
    for (auto [index, value] : std::views::enumerate(values)) {
        weighted += static_cast<long long>(index) * value;
    }

    std::vector<int> left{1, 2, 3};
    std::vector<int> right{4, 5, 6};
    int zipped = 0;
    for (auto [a, b] : std::views::zip(left, right)) {
        zipped += a * b;
    }

    int strided = 0;
    for (int value : values | std::views::stride(2)) {
        strided += value;
    }

    return even_squares.size() == 2 && weighted == 70 && zipped == 32 && strided == 9 ? 0 : 1;
}
CPP
            ;;
        expected)
            cat > "$file" <<'CPP'
#include <expected>
#include <string>
std::expected<int, std::string> parse() { return 42; }
int main() {
    auto value = parse();
    return value && *value == 42 ? 0 : 1;
}
CPP
            ;;
        expected-monadic)
            cat > "$file" <<'CPP'
#include <expected>
#include <string>
std::expected<int, std::string> parse() { return 21; }
int main() {
    auto value = parse()
        .and_then([](int x) -> std::expected<int, std::string> { return x * 2; })
        .transform([](int x) { return x + 1; });
    return value.value_or(0) == 43 ? 0 : 1;
}
CPP
            ;;
        jthread)
            cat > "$file" <<'CPP'
#include <stop_token>
#include <thread>
int main() {
    std::stop_source source;
    std::jthread worker([] {});
    source.request_stop();
    return worker.joinable() ? 0 : 1;
}
CPP
            ;;
        execution)
            cat > "$file" <<'CPP'
#include <algorithm>
#include <execution>
#include <vector>
int main() {
    std::vector<int> values(16, 1);
    std::for_each(std::execution::seq, values.begin(), values.end(), [](int& x) { x += 1; });
    std::for_each(std::execution::par_unseq, values.begin(), values.end(), [](int& x) { x += 1; });
    return values.front() == 3 ? 0 : 1;
}
CPP
            ;;
        generator)
            cat > "$file" <<'CPP'
#include <generator>
std::generator<int> numbers() {
    co_yield 1;
    co_yield 2;
}
int main() {
    int sum = 0;
    for (int value : numbers()) sum += value;
    return sum == 3 ? 0 : 1;
}
CPP
            ;;
        int128)
            cat > "$file" <<'CPP'
int main() {
    __int128 value = 1;
    value <<= 80;
    return value > 0 ? 0 : 1;
}
CPP
            ;;
        posix-mmap)
            cat > "$file" <<'CPP'
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
int main() {
    return 0;
}
CPP
            ;;
        *)
            return 1
            ;;
    esac
}

run_feature_test() {
    local id="$1"
    local feature="$2"
    local compiler="${TOOLCHAIN_COMPILER[$id]}"
    local probe_dir="$BUILD_DIR/feature-tests/$id"
    local source="$probe_dir/$feature.cpp"
    local output="$probe_dir/$feature"
    local error_file="$probe_dir/$feature.err"
    local -a cxxflags
    local -a ldflags

    mkdir -p "$probe_dir" || return 1
    feature_source "$feature" "$source"

    if [ "${TOOLCHAIN_KIND[$id]}" = "msvc" ]; then
        msvc_compile "${TOOLCHAIN_COMPILER[$id]}" "$source" "$output" "$error_file" "${TOOLCHAIN_CXXFLAGS[$id]}"
        return
    fi

    read -r -a cxxflags <<< "${TOOLCHAIN_CXXFLAGS[$id]}"
    read -r -a ldflags <<< "${TOOLCHAIN_LDFLAGS[$id]}"

    "$compiler" "${cxxflags[@]}" "$source" -o "$output" "${ldflags[@]}" >"$error_file" 2>&1
}

probe_toolchain() {
    local id="$1"
    local feature
    local caps=""
    local version

    log ""
    log "Toolchain: $id (${TOOLCHAIN_LABEL[$id]})"
    log "Compiler: ${TOOLCHAIN_COMPILER[$id]}"
    if [ "${TOOLCHAIN_KIND[$id]}" = "msvc" ]; then
        version="$(msvc_version "${TOOLCHAIN_COMPILER[$id]}" || true)"
    else
        version="$("${TOOLCHAIN_COMPILER[$id]}" --version 2>/dev/null | head -n 1 || true)"
    fi
    log "Version: ${version:-unknown}"
    log "CXXFLAGS: ${TOOLCHAIN_CXXFLAGS[$id]}"
    log "LDFLAGS: ${TOOLCHAIN_LDFLAGS[$id]:-}"

    for feature in print format ranges ranges23 expected expected-monadic jthread execution generator int128 posix-mmap; do
        if run_feature_test "$id" "$feature"; then
            caps="$caps $feature"
            log "  ok   $feature"
        else
            log "  miss $feature"
            log "       $(first_diagnostic "$BUILD_DIR/feature-tests/$id/$feature.err")"
        fi
    done

    TOOLCHAIN_CAPS["$id"]="$caps"
}

has_capability() {
    local id="$1"
    local feature="$2"
    [[ " ${TOOLCHAIN_CAPS[$id]} " == *" $feature "* ]]
}

source_requires() {
    local file="$1"
    local required=""

    if grep -Eq '#include <print>|std::print|std::println' "$file"; then
        required="$required print"
    fi
    if grep -Eq '#include <format>|std::format' "$file"; then
        required="$required format"
    fi
    if grep -Eq '#include <ranges>|std::ranges|std::views' "$file"; then
        required="$required ranges"
    fi
    if grep -Eq 'std::views::(enumerate|zip|stride)|std::ranges::to' "$file"; then
        required="$required ranges23"
    fi
    if grep -Eq '#include <expected>|std::expected' "$file"; then
        required="$required expected"
    fi
    if grep -Eq '\.(and_then|transform|or_else)\(' "$file" && grep -Eq '#include <expected>|std::expected' "$file"; then
        required="$required expected-monadic"
    fi
    if grep -Eq '#include <stop_token>|std::jthread|std::stop_' "$file"; then
        required="$required jthread"
    fi
    if grep -Eq '#include <execution>|std::execution' "$file"; then
        required="$required execution"
    fi
    if grep -Eq '#include <generator>|std::generator' "$file"; then
        required="$required generator"
    fi
    if grep -Eq '^[[:space:]]*(const[[:space:]]+)?(__uint128_t|__int128)[[:space:]*&]+[A-Za-z_]|^[[:space:]]*unsigned[[:space:]]+__int128[[:space:]*&]+[A-Za-z_]|static_cast<(__uint128_t|unsigned[[:space:]]+__int128|__int128)>' "$file"; then
        required="$required int128"
    fi
    case "$file" in
        */cap03/04-mmap.cpp)
            required="$required posix-mmap"
            ;;
    esac

    printf '%s\n' "$required" | xargs -n1 2>/dev/null | awk '!seen[$0]++' | xargs 2>/dev/null || true
}

toolchain_supports_source() {
    local id="$1"
    local required="$2"
    local feature

    for feature in $required; do
        if ! has_capability "$id" "$feature"; then
            return 1
        fi
    done
}

ordered_toolchains() {
    local wanted
    local id
    local seen=""

    for wanted in $TOOLCHAIN_ORDER; do
        for id in "${TOOLCHAIN_IDS[@]}"; do
            if [ "$id" = "$wanted" ] && [[ " $seen " != *" $id "* ]]; then
                printf '%s\n' "$id"
                seen="$seen $id"
            fi
        done
    done

    for id in "${TOOLCHAIN_IDS[@]}"; do
        if [[ " $seen " != *" $id "* ]]; then
            printf '%s\n' "$id"
        fi
    done
}

compile_with_toolchain() {
    local id="$1"
    local source="$2"
    local output="$3"
    local error_file="$4"
    local compiler="${TOOLCHAIN_COMPILER[$id]}"
    local -a cxxflags
    local -a ldflags

    if [ "${TOOLCHAIN_KIND[$id]}" = "msvc" ]; then
        msvc_compile "$compiler" "$source" "$output" "$error_file" "${TOOLCHAIN_CXXFLAGS[$id]}"
        return
    fi

    read -r -a cxxflags <<< "${TOOLCHAIN_CXXFLAGS[$id]}"
    read -r -a ldflags <<< "${TOOLCHAIN_LDFLAGS[$id]}"

    "$compiler" "${cxxflags[@]}" "$source" -o "$output" "${ldflags[@]}" >"$error_file" 2>&1
}

compile_repository() {
    local -a files
    local -a candidates
    local total
    local failed=0
    local passed=0
    local index=0
    local file
    local rel
    local output
    local error_file
    local required
    local id
    local compiled

    mapfile -t files < <(
        find "$ROOT_DIR" \
            -path "$BUILD_DIR" -prune -o \
            -path "$ROOT_DIR/.compile-check" -prune -o \
            -path "$ROOT_DIR/.compile-check-msvc" -prune -o \
            -path "$ROOT_DIR/.bench-msvc" -prune -o \
            -type f -name '*.cpp' -print | sort
    )
    total="${#files[@]}"

    log ""
    log "Found $total .cpp files."
    log ""

    for file in "${files[@]}"; do
        index=$((index + 1))
        rel="${file#$ROOT_DIR/}"
        output="$BUILD_DIR/${rel%.cpp}"
        error_file="$BUILD_DIR/${rel%.cpp}.err"
        required="$(source_requires "$file")"
        compiled=0
        candidates=()

        while IFS= read -r id; do
            if [ -z "$id" ]; then
                continue
            fi
            if toolchain_supports_source "$id" "$required"; then
                candidates+=("$id")
            fi
        done < <(ordered_toolchains)

        mkdir -p "$(dirname "$output")"
        printf '[%d/%d] %s\n' "$index" "$total" "$rel"

        if [ "${#candidates[@]}" -eq 0 ]; then
            failed=$((failed + 1))
            log "FAILED: $rel"
            log "Required features: ${required:-none}"
            log "No discovered toolchain supports all required features."
            log ""
            continue
        fi

        for id in "${candidates[@]}"; do
            if compile_with_toolchain "$id" "$file" "$output" "$error_file"; then
                passed=$((passed + 1))
                TOOLCHAIN_SELECTED_COUNT["$id"]=$((TOOLCHAIN_SELECTED_COUNT["$id"] + 1))
                log "OK: $rel"
                log "Toolchain: $id"
                log "Required features: ${required:-none}"
                log ""
                compiled=1
                break
            fi

            log "RETRY: $rel failed with $id"
            log "First diagnostic: $(first_diagnostic "$error_file")"
        done

        if [ "$compiled" -eq 0 ]; then
            failed=$((failed + 1))
            log "FAILED: $rel"
            log "Required features: ${required:-none}"
            log "Tried toolchains: ${candidates[*]}"
            log "Full diagnostic: $error_file"
            log ""
        fi
    done

    log ""
    log "Compilation finished: $passed passed, $failed failed."
    log "Toolchain usage:"
    for id in "${TOOLCHAIN_IDS[@]}"; do
        log "  $id: ${TOOLCHAIN_SELECTED_COUNT[$id]}"
    done

    if [ "$failed" -ne 0 ]; then
        return 1
    fi
}

if [ "$CLEAN" -eq 1 ]; then
    ROOT_ABS="$(realpath -m "$ROOT_DIR")"
    BUILD_ABS="$(realpath -m "$BUILD_DIR")"
    case "$BUILD_ABS" in
        "$ROOT_ABS"/*)
            rm -rf "$BUILD_ABS"
            BUILD_DIR="$BUILD_ABS"
            ;;
        *)
            printf 'Refusing to clean BUILD_DIR outside the repository: %s\n' "$BUILD_ABS" >&2
            exit 1
            ;;
    esac
fi

if ! mkdir -p "$BUILD_DIR"; then
    printf 'Cannot create build directory: %s\n' "$BUILD_DIR" >&2
    exit 1
fi

if ! {
    printf 'Compilation report - %s\n' "$(date)"
    printf 'Root: %s\n' "$ROOT_DIR"
    printf 'Build directory: %s\n' "$BUILD_DIR"
    printf 'Toolchain order: %s\n\n' "$TOOLCHAIN_ORDER"
} > "$REPORT"; then
    printf 'Cannot write compilation report: %s\n' "$REPORT" >&2
    exit 1
fi

if ! grep -qi microsoft /proc/version 2>/dev/null; then
    log "Note: /proc/version does not look like WSL. Continuing as a Linux build."
fi

discover_toolchains

if [ "${#TOOLCHAIN_IDS[@]}" -eq 0 ]; then
    log "No C++ compiler was found. Install LLVM/Clang in WSL and rerun:"
    log "  sudo apt install clang libc++-dev libc++abi-dev"
    exit 1
fi

log "Discovered toolchains: ${TOOLCHAIN_IDS[*]}"

for id in "${TOOLCHAIN_IDS[@]}"; do
    probe_toolchain "$id"
done

log ""
log "Report: $REPORT"
log "Build directory: $BUILD_DIR"

if [ "$PREFLIGHT_ONLY" -eq 1 ]; then
    log "Preflight finished."
    exit 0
fi

compile_repository
