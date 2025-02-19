# Chin Lun Wong cw0710
NR > 1 {

    missing = 0
    total = 0
    count = 0

    for (i = 3; i <= NF; i++) {
        if ($i == "--") {
            missing = 1
            $i = "--"
        } else {
            total += $i
            count++
        }
    }

    if (missing) {
        print $1, $2, $3, $4, $5, "=>", "F (missing scores)"
    } else {
        average = total / count

        if (average >= 89.5) {
            grade = "A"
        } else if (average >= 79.5) {
            grade = "B"
        } else if (average >= 69.5) {
            grade = "C"
        } else if (average >= 59.5) {
            grade = "D"
        } else {
            grade = "F"
        }

        print $1, $2, $3, $4, $5, "=>", grade
    }
}
