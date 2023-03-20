import { useCourseStore } from "@/pinia/modules/course"; 
import { store } from '@/pinia/enter'

// 所有课程数据存储的地方
export const CourseStore = new useCourseStore(store)