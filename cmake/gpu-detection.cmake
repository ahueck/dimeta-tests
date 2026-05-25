function(ditests_cuda_runnable_detect cuda_is_runnable)
  set(${cuda_is_runnable} 0 PARENT_SCOPE)

  find_program(DITESTS_NVIDIA_SMI_EXEC nvidia-smi)
  mark_as_advanced(DITESTS_NVIDIA_SMI_EXEC)
  if(NOT DITESTS_NVIDIA_SMI_EXEC)
    return()
  endif()

  execute_process(
    COMMAND ${DITESTS_NVIDIA_SMI_EXEC} -L
    RESULT_VARIABLE smi_result
    OUTPUT_VARIABLE smi_out
    ERROR_QUIET
  )
  if(NOT smi_result EQUAL 0)
    return()
  endif()

  string(REGEX MATCHALL "GPU [0-9]+:" smi_gpu_list "${smi_out}")
  list(LENGTH smi_gpu_list smi_gpu_count)
  if(smi_gpu_count GREATER 0)
    set(${cuda_is_runnable} 1 PARENT_SCOPE)
  endif()
endfunction()

ditests_cuda_runnable_detect(DITESTS_CUDA_RUNNABLE)

function(ditests_hip_runnable_detect hip_is_runnable hip_arch)
  set(${hip_is_runnable} 0 PARENT_SCOPE)
  set(${hip_arch} "" PARENT_SCOPE)

  find_program(DITESTS_ROCM_SMI_EXEC rocm-smi)
  mark_as_advanced(DITESTS_ROCM_SMI_EXEC)
  if(NOT DITESTS_ROCM_SMI_EXEC)
    return()
  endif()

  execute_process(
    COMMAND ${DITESTS_ROCM_SMI_EXEC} --showproductname --showuniqueid --json
    RESULT_VARIABLE smi_result
    OUTPUT_VARIABLE smi_out
    ERROR_QUIET
  )
  if(NOT smi_result EQUAL 0)
    return()
  endif()

  string(FIND "${smi_out}" "{" json_start)
  if(json_start EQUAL -1)
    return()
  endif()

  string(SUBSTRING "${smi_out}" ${json_start} -1 clean_json)
  string(JSON smi_gpu_count LENGTH "${clean_json}")

  if(smi_gpu_count GREATER 0)
    math(EXPR loop_end "${smi_gpu_count} - 1")
    foreach(i RANGE ${loop_end})
      string(JSON device_key MEMBER "${clean_json}" ${i})
      if(device_key MATCHES "^(card|device)[0-9]+")
        set(${hip_is_runnable} 1 PARENT_SCOPE)
        string(JSON current_arch GET "${clean_json}" "${device_key}" "GFX Version")
        set(${hip_arch} "${current_arch}" PARENT_SCOPE)
        return()
      endif()
    endforeach()
  endif()
endfunction()

ditests_hip_runnable_detect(DITESTS_HIP_RUNNABLE DITESTS_HIP_ARCH)
