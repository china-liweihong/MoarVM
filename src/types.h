/* struct and union types are forward-declared for convenience */

typedef struct MVMActiveHandler MVMActiveHandler;
typedef struct MVMArgInfo MVMArgInfo;
typedef struct MVMArgProcContext MVMArgProcContext;
typedef struct MVMArray MVMArray;
typedef struct MVMArrayBody MVMArrayBody;
typedef struct MVMArrayREPRData MVMArrayREPRData;
typedef struct MVMAsyncTask MVMAsyncTask;
typedef struct MVMAsyncTaskBody MVMAsyncTaskBody;
typedef struct MVMAsyncTaskOps MVMAsyncTaskOps;
typedef struct MVMAttributeIdentifier MVMAttributeIdentifier;
typedef struct MVMBoolificationSpec MVMBoolificationSpec;
typedef struct MVMBootTypes MVMBootTypes;
typedef struct MVMBytecodeAnnotation MVMBytecodeAnnotation;
typedef struct MVMCallCapture MVMCallCapture;
typedef struct MVMCallCaptureBody MVMCallCaptureBody;
typedef struct MVMCallsite MVMCallsite;
typedef struct MVMCallsiteInterns MVMCallsiteInterns;
typedef struct MVMCallStackRegion MVMCallStackRegion;
typedef struct MVMCFunction MVMCFunction;
typedef struct MVMCFunctionBody MVMCFunctionBody;
typedef struct MVMCode MVMCode;
typedef struct MVMCodeBody MVMCodeBody;
typedef struct MVMCollectable MVMCollectable;
typedef struct MVMCompUnit MVMCompUnit;
typedef struct MVMCompUnitBody MVMCompUnitBody;
typedef struct MVMConcatState MVMConcatState;
typedef struct MVMContainerConfigurer MVMContainerConfigurer;
typedef struct MVMContainerSpec MVMContainerSpec;
typedef struct MVMContainerRegistry MVMContainerRegistry;
typedef struct MVMContext MVMContext;
typedef struct MVMContextBody MVMContextBody;
typedef struct MVMContinuationTag MVMContinuationTag;
typedef struct MVMDecoder MVMDecoder;
typedef struct MVMDecoderBody MVMDecoderBody;
typedef struct MVMDLLRegistry MVMDLLRegistry;
typedef struct MVMDLLSym MVMDLLSym;
typedef struct MVMDLLSymBody MVMDLLSymBody;
typedef struct MVMException MVMException;
typedef struct MVMExceptionBody MVMExceptionBody;
typedef struct MVMExtOpRecord MVMExtOpRecord;
typedef struct MVMExtOpRegistry MVMExtOpRegistry;
typedef struct MVMExtRegistry MVMExtRegistry;
typedef struct MVMFixedSizeAlloc MVMFixedSizeAlloc;
typedef struct MVMFixedSizeAllocFreeListEntry MVMFixedSizeAllocFreeListEntry;
typedef struct MVMFixedSizeAllocSafepointFreeListEntry MVMFixedSizeAllocSafepointFreeListEntry;
typedef struct MVMFixedSizeAllocSizeClass MVMFixedSizeAllocSizeClass;
typedef struct MVMFrame MVMFrame;
typedef struct MVMFrameHandler MVMFrameHandler;
typedef struct MVMGen2Allocator MVMGen2Allocator;
typedef struct MVMGen2SizeClass MVMGen2SizeClass;
typedef struct MVMGCPassedWork MVMGCPassedWork;
typedef struct MVMGCWorklist MVMGCWorklist;
typedef struct MVMHash MVMHash;
typedef struct MVMHashAttrStore MVMHashAttrStore;
typedef struct MVMHashAttrStoreBody MVMHashAttrStoreBody;
typedef struct MVMHashBody MVMHashBody;
typedef struct MVMHashEntry MVMHashEntry;
typedef struct MVMHLLConfig MVMHLLConfig;
typedef struct MVMIntConstCache MVMIntConstCache;
typedef struct MVMInstance MVMInstance;
typedef struct MVMInvocationSpec MVMInvocationSpec;
typedef struct MVMIter MVMIter;
typedef struct MVMIterBody MVMIterBody;
typedef struct MVMKnowHOWAttributeREPR MVMKnowHOWAttributeREPR;
typedef struct MVMKnowHOWAttributeREPRBody MVMKnowHOWAttributeREPRBody;
typedef struct MVMKnowHOWREPR MVMKnowHOWREPR;
typedef struct MVMKnowHOWREPRBody MVMKnowHOWREPRBody;
typedef struct MVMLexicalRegistry MVMLexicalRegistry;
typedef struct MVMLexotic MVMLexotic;
typedef struct MVMLexoticBody MVMLexoticBody;
typedef struct MVMLoadedCompUnitName MVMLoadedCompUnitName;
typedef struct MVMNFA MVMNFA;
typedef struct MVMNFABody MVMNFABody;
typedef struct MVMNFAStateInfo MVMNFAStateInfo;
typedef struct MVMNFGState MVMNFGState;
typedef struct MVMNFGSynthetic MVMNFGSynthetic;
typedef struct MVMNFGTrieNode MVMNFGTrieNode;
typedef struct MVMNGFTrieNodeEntry MVMNGFTrieNodeEntry;
typedef struct MVMNativeCall MVMNativeCall;
typedef struct MVMNativeCallBody MVMNativeCallBody;
typedef struct MVMNativeRef MVMNativeRef;
typedef struct MVMNativeRefBody MVMNativeRefBody;
typedef struct MVMNativeRefREPRData MVMNativeRefREPRData;
typedef struct MVMNormalizer MVMNormalizer;
typedef struct MVMNull MVMNull;
typedef struct MVMNullBody MVMNullBody;
typedef struct MVMCStr MVMCStr;
typedef struct MVMCStrBody MVMCStrBody;
typedef struct MVMCPointer MVMCPointer;
typedef struct MVMCPointerBody MVMCPointerBody;
typedef struct MVMCArray MVMCArray;
typedef struct MVMCArrayBody MVMCArrayBody;
typedef struct MVMCArrayREPRData MVMCArrayREPRData;
typedef struct MVMCStruct MVMCStruct;
typedef struct MVMCStructBody MVMCStructBody;
typedef struct MVMCStructNameMap MVMCStructNameMap;
typedef struct MVMCStructREPRData MVMCStructREPRData;
typedef struct MVMCPPStruct MVMCPPStruct;
typedef struct MVMCPPStructBody MVMCPPStructBody;
typedef struct MVMCPPStructNameMap MVMCPPStructNameMap;
typedef struct MVMCPPStructREPRData MVMCPPStructREPRData;
typedef struct MVMCUnion MVMCUnion;
typedef struct MVMCUnionBody MVMCUnionBody;
typedef struct MVMCUnionNameMap MVMCUnionNameMap;
typedef struct MVMCUnionREPRData MVMCUnionREPRData;
typedef struct MVMMultiCache MVMMultiCache;
typedef struct MVMMultiCacheBody MVMMultiCacheBody;
typedef struct MVMMultiCacheNode MVMMultiCacheNode;
typedef struct MVMMultiDimArray MVMMultiDimArray;
typedef struct MVMMultiDimArrayBody MVMMultiDimArrayBody;
typedef struct MVMMultiDimArrayREPRData MVMMultiDimArrayREPRData;
typedef struct MVMContinuation MVMContinuation;
typedef struct MVMContinuationBody MVMContinuationBody;
typedef struct MVMReentrantMutex MVMReentrantMutex;
typedef struct MVMReentrantMutexBody MVMReentrantMutexBody;
typedef struct MVMConditionVariable MVMConditionVariable;
typedef struct MVMConditionVariableBody MVMConditionVariableBody;
typedef struct MVMSemaphore MVMSemaphore;
typedef struct MVMSemaphoreBody MVMSemaphoreBody;
typedef struct MVMConcBlockingQueue MVMConcBlockingQueue;
typedef struct MVMConcBlockingQueueBody MVMConcBlockingQueueBody;
typedef struct MVMConcBlockingQueueNode MVMConcBlockingQueueNode;
typedef struct MVMConcBlockingQueueLocks MVMConcBlockingQueueLocks;
typedef struct MVMObject MVMObject;
typedef struct MVMObjectId MVMObjectId;
typedef struct MVMObjectStooge MVMObjectStooge;
typedef struct MVMOpInfo MVMOpInfo;
typedef struct MVMOSHandle MVMOSHandle;
typedef struct MVMOSHandleBody MVMOSHandleBody;
typedef struct MVMP6bigint MVMP6bigint;
typedef struct MVMP6bigintBody MVMP6bigintBody;
typedef struct MVMP6int MVMP6int;
typedef struct MVMP6intBody MVMP6intBody;
typedef struct MVMP6intREPRData MVMP6intREPRData;
typedef struct MVMP6num MVMP6num;
typedef struct MVMP6numBody MVMP6numBody;
typedef struct MVMP6numREPRData MVMP6numREPRData;
typedef struct MVMP6opaque MVMP6opaque;
typedef struct MVMP6opaqueBody MVMP6opaqueBody;
typedef struct MVMP6opaqueBoxedTypeMap MVMP6opaqueBoxedTypeMap;
typedef struct MVMP6opaqueNameMap MVMP6opaqueNameMap;
typedef struct MVMP6opaqueREPRData MVMP6opaqueREPRData;
typedef struct MVMP6str MVMP6str;
typedef struct MVMP6strBody MVMP6strBody;
typedef union  MVMRegister MVMRegister;
typedef struct MVMReprRegistry MVMReprRegistry;
typedef struct MVMREPROps MVMREPROps;
typedef struct MVMREPROps_Associative MVMREPROps_Associative;
typedef struct MVMREPROps_Attribute MVMREPROps_Attribute;
typedef struct MVMREPROps_Boxing MVMREPROps_Boxing;
typedef struct MVMREPROps_Positional MVMREPROps_Positional;
typedef struct MVMSerializationContext MVMSerializationContext;
typedef struct MVMSerializationContextBody MVMSerializationContextBody;
typedef struct MVMSerializationReader MVMSerializationReader;
typedef struct MVMDeserializeWorklist MVMDeserializeWorklist;
typedef struct MVMSerializationRoot MVMSerializationRoot;
typedef struct MVMSerializationWriter MVMSerializationWriter;
typedef struct MVMSpeshGraph MVMSpeshGraph;
typedef struct MVMSpeshMemBlock MVMSpeshMemBlock;
typedef struct MVMSpeshTemporary MVMSpeshTemporary;
typedef struct MVMSpeshBB MVMSpeshBB;
typedef struct MVMSpeshIns MVMSpeshIns;
typedef union MVMSpeshOperand MVMSpeshOperand;
typedef struct MVMSpeshAnn MVMSpeshAnn;
typedef struct MVMSpeshFacts MVMSpeshFacts;
typedef struct MVMSpeshCode MVMSpeshCode;
typedef struct MVMSpeshCandidate MVMSpeshCandidate;
typedef struct MVMSpeshGuard MVMSpeshGuard;
typedef struct MVMSpeshLogGuard MVMSpeshLogGuard;
typedef struct MVMSpeshCallInfo MVMSpeshCallInfo;
typedef struct MVMSpeshInline MVMSpeshInline;
typedef struct MVMSpeshIterator MVMSpeshIterator;
typedef struct MVMSTable MVMSTable;
typedef struct MVMStaticFrame MVMStaticFrame;
typedef struct MVMStaticFrameBody MVMStaticFrameBody;
typedef struct MVMStaticFrameInstrumentation MVMStaticFrameInstrumentation;
typedef struct MVMStorageSpec MVMStorageSpec;
typedef struct MVMString MVMString;
typedef struct MVMStringBody MVMStringBody;
typedef struct MVMStringConsts MVMStringConsts;
typedef struct MVMStringStrand MVMStringStrand;
typedef struct MVMGraphemeIter MVMGraphemeIter;
typedef struct MVMCodepointIter MVMCodepointIter;
typedef struct MVMThread MVMThread;
typedef struct MVMThreadBody MVMThreadBody;
typedef struct MVMThreadContext MVMThreadContext;
typedef struct MVMUnicodeNamedValue MVMUnicodeNamedValue;
typedef struct MVMUnicodeNameRegistry MVMUnicodeNameRegistry;
typedef struct MVMUninstantiable MVMUninstantiable;
typedef struct MVMWorkThread MVMWorkThread;
typedef struct MVMIOOps MVMIOOps;
typedef struct MVMIOClosable MVMIOClosable;
typedef struct MVMIOEncodable MVMIOEncodable;
typedef struct MVMIOSyncReadable MVMIOSyncReadable;
typedef struct MVMIOSyncWritable MVMIOSyncWritable;
typedef struct MVMIOAsyncReadable MVMIOAsyncReadable;
typedef struct MVMIOAsyncWritable MVMIOAsyncWritable;
typedef struct MVMIOAsyncWritableTo MVMIOAsyncWritableTo;
typedef struct MVMIOSeekable MVMIOSeekable;
typedef struct MVMIOSockety MVMIOSockety;
typedef struct MVMIOPipeable MVMIOPipeable;
typedef struct MVMIOIntrospection MVMIOIntrospection;
typedef struct MVMIOLockable MVMIOLockable;
typedef struct MVMIOSyncStreamData MVMIOSyncStreamData;
typedef struct MVMIOSyncPipeData MVMIOSyncPipeData;
typedef struct MVMDecodeStream MVMDecodeStream;
typedef struct MVMDecodeStreamBytes MVMDecodeStreamBytes;
typedef struct MVMDecodeStreamChars MVMDecodeStreamChars;
typedef struct MVMDecodeStreamSeparators MVMDecodeStreamSeparators;
typedef struct MVMNativeCallback MVMNativeCallback;
typedef struct MVMNativeCallbackCacheHead MVMNativeCallbackCacheHead;
typedef struct MVMJitGraph MVMJitGraph;
typedef struct MVMJitNode MVMJitNode;
typedef struct MVMJitDeopt MVMJitDeopt;
typedef struct MVMJitInline MVMJitInline;
typedef struct MVMJitHandler MVMJitHandler;
typedef struct MVMJitPrimitive MVMJitPrimitive;
typedef struct MVMJitBranch MVMJitBranch;
typedef struct MVMJitCallC MVMJitCallC;
typedef struct MVMJitCallArg MVMJitCallArg;
typedef struct MVMJitLabel MVMJitLabel;
typedef struct MVMJitGuard MVMJitGuard;
typedef struct MVMJitInvoke MVMJitInvoke;
typedef struct MVMJitJumpList MVMJitJumpList;
typedef struct MVMJitControl MVMJitControl;
typedef struct MVMJitCode MVMJitCode;
typedef struct MVMJitExprTree MVMJitExprTree;
typedef struct MVMJitTreeTraverser MVMJitTreeTraverser;
typedef struct MVMJitCompiler MVMJitCompiler;
typedef struct MVMJitExprTemplate MVMJitExprTemplate;
typedef struct MVMJitExprOpInfo MVMJitExprOpInfo;
typedef struct MVMJitExprNodeInfo MVMJitExprNodeInfo;
typedef struct MVMJitTileTemplate MVMJitTileTemplate;
typedef struct MVMJitTile MVMJitTile;
typedef struct MVMJitTileList MVMJitTileList;
typedef struct MVMJitValue MVMJitValue;
typedef struct MVMProfileThreadData MVMProfileThreadData;
typedef struct MVMProfileGC MVMProfileGC;
typedef struct MVMProfileCallNode MVMProfileCallNode;
typedef struct MVMProfileAllocationCount MVMProfileAllocationCount;
typedef struct MVMProfileContinuationData MVMProfileContinuationData;
typedef struct MVMHeapSnapshotCollection MVMHeapSnapshotCollection;
typedef struct MVMHeapSnapshot MVMHeapSnapshot;
typedef struct MVMHeapSnapshotType MVMHeapSnapshotType;
typedef struct MVMHeapSnapshotStaticFrame MVMHeapSnapshotStaticFrame;
typedef struct MVMHeapSnapshotCollectable MVMHeapSnapshotCollectable;
typedef struct MVMHeapSnapshotReference MVMHeapSnapshotReference;
typedef struct MVMHeapSnapshotState MVMHeapSnapshotState;
typedef struct MVMHeapSnapshotWorkItem MVMHeapSnapshotWorkItem;
typedef struct MVMHeapSnapshotSeen MVMHeapSnapshotSeen;
