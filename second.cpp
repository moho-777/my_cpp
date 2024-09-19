template <>
InstrumentBatchRequest GetBatchInstrumentRequestWithDetails(
    const InstrumentBatchRequest& originalRequest, 
    int batchIndex, 
    int chunkSize) {

    InstrumentBatchRequest newRequest = originalRequest;

    // Set batchIndex and batchTotal in the header
    newRequest.mutable_header()->set_batchindex(batchIndex);
    newRequest.mutable_header()->set_batchtotal((originalRequest.requests_size() + chunkSize - 1) / chunkSize);

    // Copy the InstrumentActionType and BatchActionMode from the original request
    newRequest.set_instrumentactiontype(originalRequest.instrumentactiontype());
    newRequest.set_batchactionmode(originalRequest.batchactionmode());

    // Chunk the actual requests into sub-batches
    int totalRequests = originalRequest.requests_size();
    int start = batchIndex * chunkSize;
    int end = std::min(start + chunkSize, totalRequests);

    newRequest.clear_requests(); // Clear existing requests in newRequest
    for (int i = start; i < end; ++i) {
        *newRequest.add_requests() = originalRequest.requests(i); // Add chunked requests
    }

    return newRequest;
}

template <typename RequestType>
RequestType GetBatchInstrumentRequestWithDetails(
    const RequestType& originalRequest, 
    int batchIndex, 
    int chunkSize) {

    RequestType newRequest = originalRequest;

    newRequest.mutable_header()->set_batchindex(batchIndex);
    newRequest.mutable_header()->set_batchtotal((originalRequest.requests_size() + chunkSize - 1) / chunkSize);

    return newRequest;
}

template <>
AnotherBatchRequest GetBatchInstrumentRequestWithDetails(
    const AnotherBatchRequest& originalRequest, 
    int batchIndex, 
    int chunkSize) {

    AnotherBatchRequest newRequest = originalRequest;

    // Set batchIndex and batchTotal in the header
    newRequest.mutable_header()->set_batchindex(batchIndex);
    newRequest.mutable_header()->set_batchtotal((originalRequest.requests_size() + chunkSize - 1) / chunkSize);

    // Specific fields of AnotherBatchRequest
    newRequest.set_customfield(originalRequest.customfield());  // Example custom field in this request type

    // Chunk the actual requests into sub-batches
    int totalRequests = originalRequest.requests_size();
    int start = batchIndex * chunkSize;
    int end = std::min(start + chunkSize, totalRequests);

    newRequest.clear_requests();  // Clear existing requests in newRequest
    for (int i = start; i < end; ++i) {
        *newRequest.add_requests() = originalRequest.requests(i);  // Add chunked requests
    }

    return newRequest;
}

