import { defineStore } from 'pinia'
import { CourseTable } from '@/api/course'
import { ref } from 'vue'

export const useCourseStore = defineStore('course', () => {
    const courseList = ref([])
    const GetCourseTable = async () => {
        try {
            const res = await CourseTable()

            if (res.data.status_code == 0) {
                courseList.value = res.data.course_list
                return true
            }
            return false
        } catch (err) {
            return false
        }   
    }

    return {
        courseList,
        GetCourseTable
    }
})
