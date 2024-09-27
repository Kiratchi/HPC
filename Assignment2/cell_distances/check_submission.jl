#!/usr/bin/julia

const hpc_folder = dirname(dirname(@__FILE__))

include("check_submission_incl.jl")

check(ARGS, CheckCaseCellDistances[
  CheckCaseCellDistances("1e4", true, HyperfineSetup(  1,    0.36, 5., 10, 1)),
  CheckCaseCellDistances("1e5", true,  HyperfineSetup( 5,    6.88, 5.,  2, 1)),
  CheckCaseCellDistances("1e5", true,  HyperfineSetup(10,    3.67, 5.,  2, 1)),
  CheckCaseCellDistances("1e5", true,  HyperfineSetup(20,    2.06, 5.,  4, 1)),
  # CheckCaseCellDistances("3e7", false, HyperfineSetup(40, 1000.0 , 5.,  1, 0)),
  ])
println("Submission passes the check script.")

