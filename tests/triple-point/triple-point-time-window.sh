NUM_PARALLEL_PROCESSORS=8
testNames=(offline online romhr restore)
runAbsoluteFOM="true"
absoluteFOMOptions="-p 3 -m data/box01_hex.mesh -rs 1 -tf 0.04 -cfl 0.05 -pa -print"
absoluteFOMTol="1e-6"
absoluteFOMTolParallel="1e-1"
absoluteRelErrorTol="1"
absoluteRelErrorTolParallel="1"
speedupPercentTol="200"
case $subTestNum in
  1)
    $LAGHOS -p 3 -m data/box01_hex.mesh -rs 1 -tf 0.04 -cfl 0.05 -pa -offline -writesol -romsvds -ef 0.9999 -nwin 4 -tw "$BASE_DIR"/tests/triple-point/triple-point-time-window.csv
    ;;
  2)
    $LAGHOS -p 3 -m data/box01_hex.mesh -rs 1 -tf 0.04 -cfl 0.05 -pa -online -soldiff -nwin 4 -twp twpTemp.csv
    ;;
  3)
    $LAGHOS -p 3 -m data/box01_hex.mesh -rs 1 -tf 0.04 -cfl 0.05 -pa -online -soldiff -nwin 4 -romhr -twp twpTemp.csv
    ;;
  4)
    $LAGHOS -p 3 -m data/box01_hex.mesh -rs 1 -tf 0.04 -cfl 0.05 -pa -restore -nwin 4 -twp twpTemp.csv
    ;;
esac
