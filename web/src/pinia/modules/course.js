import { defineStore } from 'pinia'
import { CourseTable } from '@/api/course'
import { ref } from 'vue'

export const useCourseStore = defineStore('course', () => {
    const rdata = ref(null)
    const GetCourseTable = async () => {
        try {
            const res = await CourseTable()

            if (res.data.status_code == 0) {
                rdata.value = res.data
                return true
            }
            return false
        } catch (err) {
            return false
        }   
    }

    return {
        rdata,
        GetCourseTable
    }
})
